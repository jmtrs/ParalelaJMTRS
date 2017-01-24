#include <sys/stat.h>//para coger los datos del archivo
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>//Libreria para tener diferentes opciones
#include <stdarg.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>

#define BUFFER 2048
#define TIPO "-"
#define KB 1024
#define MB (KB * KB)
#define GB (MB * KB)

struct linea {
    int all; /* todos incluidos . */
    int listar; /* listar */
    char **files; /* nombres de archivos */
    int count; /* numero de archivos */
};

static struct linea token; /* inicialmente 0s */
static const char *opc = "al";
static const struct option options[] = {
    {"all", no_argument, NULL, 'a'},
    {"listar", no_argument, NULL, 'l'},
    {"help", no_argument, NULL, '?'},
    {0, 0, 0, 0}
};


static void error(const char *fmt, ...) {
    va_list ap;
    char buf[BUFFER];

    va_start(ap, fmt);
    vsnprintf(buf, BUFFER, fmt, ap);
    snprintf(buf+strlen(buf), BUFFER-strlen(buf), ": %s", strerror(errno));
    strcat(buf, "\n");
    fflush(stdout);
    fputs(buf, stderr);
    fflush(NULL);
    va_end(ap);
}

static int comparar(const void *pa, const void *pb) {
    char *s1 = *(char**)pa;
    char *s2 = *(char**)pb;
    return strcasecmp(s1, s2);
}


static void ordenar(char **files, int n) {
    qsort(files, n, sizeof(char*), comparar);
}

static int leerTokens(int argc, char **argv) {
    int opt, index;
    static char* DEFAULT_FILES[] = {"."};

    for(;;) { //while true
        opt = getopt_long(argc, argv, opc, options, &index);
        if (opt == -1) break;
        switch (opt) {
            case 'a':
                token.all = 1;
                break;
            case 'l':
                token.listar = 1;
                break;
            case '?':
            default:
                return -1;
        }
    }
    

    token.files = argv + optind;
    token.count = argc - optind;
    if (token.count == 0) {
        token.files = DEFAULT_FILES;
        token.count = 1;
    }

    return 0;
}

static int esdir(const char *ruta) {
    struct stat buf;
    int ret = stat(ruta, &buf);
    return ret == 0 && S_ISDIR(buf.st_mode);
}

static void clasifica(char **carp, int *carpcont, char **files, int *count) {
    int i, ca, fi;

    for (i = ca = fi = 0; i < token.count; i++) {
        if (esdir(token.files[i])) {
            carp[ca++] = token.files[i];
        } else {
            files[fi++] = token.files[i];
        }
    }
    *carpcont = ca;
    *count = fi;
}

static int getstat(char *ruta, struct stat *buf) {
    if (token.listar) {
        return stat(ruta, buf);
    }
    return lstat(ruta, buf);
}

static char* file_mode(mode_t mode) {
    static char ms[1];
    enum {
        TYPE
    };

    strcpy(ms, TIPO);
    if (S_ISDIR(mode))  ms[TYPE] = 'D'; //Directorio
    else if (S_ISREG(mode)) ms[TYPE] = 'R'; //Regular
    else if (S_ISLNK(mode)) ms[TYPE] = 'L'; //Link
    else if (S_ISCHR(mode)) ms[TYPE] = 'C'; //Caracter
    else if (S_ISBLK(mode)) ms[TYPE] = 'B'; //Bloque
    else if (S_ISFIFO(mode)) ms[TYPE] = 'F'; //FIFO


    return ms;
}

static char* file_prop(uid_t uid) {
    struct passwd *pwd = getpwuid(uid);
    return (pwd == NULL) ? "" : pwd->pw_name;
}

static char* file_grupo(gid_t gid) {
    struct group *grp = getgrgid(gid);
    return (grp == NULL) ? "" : grp->gr_name;
}

static char* file_time(time_t *time) {
    static char buf[64];
    struct tm *t = localtime(time);
    strftime(buf, sizeof(buf), "%F %H:%M", t);
    return buf;
}

static char* file_name(struct stat *st, char *name) {
    char *buf;

    if (S_ISLNK(st->st_mode)) {
        ssize_t len;
        int offset;

        buf = malloc(4096);
        if (buf == NULL) {
            error("mils: no hay memoria");
            exit(1);
        }
        offset = snprintf(buf, 4096, "%s -> ", name);
        len = readlink(name, buf+offset, 4096-offset);
        if (len == -1) {
            error("mils: no hay acceso a %s", name);
            exit(1);
        }
        buf[offset+len] = '\0';
    } else {
        buf = strdup(name);
    }
    return buf;
}

static char* file_tam(off_t tam) {
    static char buf[32];

    snprintf(buf, sizeof(buf), "%7ld", tam);

    if (tam >= GB) {
        snprintf(buf, sizeof(buf), "%6.1fG", tam / (double)GB);
    } else if (tam >= MB) {
        snprintf(buf, sizeof(buf), "%6.1fM", tam / (double)MB);
    } else if (tam >= KB) {
        snprintf(buf, sizeof(buf), "%6.1fK", tam / (double)KB);
    }
    return buf;
}

static void impresion(struct stat *buf, char *name) {
    char *fname;

    
    if (token.listar == 1 ) {
        fprintf(stdout, "%-6ld ", buf->st_ino);
        fprintf(stdout, "%s ", file_mode(buf->st_mode));
        fprintf(stdout, "%-8s ", file_prop(buf->st_uid));
        fprintf(stdout, "%-8s ", file_grupo(buf->st_gid));
        fprintf(stdout, "%s ", file_tam(buf->st_size));
        fprintf(stdout, "%s ", file_time(&buf->st_mtime));
        fname = file_name(buf, name);
        fprintf(stdout, "%-s", fname);
        free(fname);
        fprintf(stdout, "\n");
    }else{
        fname = file_name(buf, name);
        fprintf(stdout, "%-s", fname);
        free(fname);
        fprintf(stdout, "\n");
    }
    
    
}

static int saltar(char *file) {
    if (token.all) {
        return 0;
    }
    if (file[0] == '.') {
        return 1;
    }
    return 0;
}

static void leerFiles(char **files, int count) {
    int i;
    struct stat buf;

    for (i = 0; i < count; i++) {
        char *file = files[i];
        if (saltar(file)) continue;
        if (getstat(file, &buf) != 0) {
            error("mils: no hay acceso a %s", file);
        } else {
            impresion(&buf, file);
        }
    }
}

static char** listarDir(char *dir, int *pn) {
    char **files;
    int n = 32, off = 0;
    DIR *dp;
    struct dirent *dirp;

    files = malloc(sizeof(char*) * n);
    if (files == NULL) {
        error("mils: no hay memoria");
        exit(1);
    }
    dp = opendir(dir);
    if (dp == NULL) {
        error("mils: no hay acceso a %s", dir);
        return NULL;
    }
    while ((dirp = readdir(dp)) != NULL) {
        if (off >= n) {
            n *= 2;
            files = realloc(files, sizeof(char*) * n);
        }
        files[off++] = strdup(dirp->d_name);
    }
    closedir(dp);
    *pn = off;
    return files;
}


static void leerDir(char **carp, int carpcont) {
    int i;
    char *cwd;

    cwd = malloc(4096 * sizeof(char));
    if (getcwd(cwd, 4096) == NULL) {
        error("mils: no hay memoria");
        exit(1);
    }

    for (i = 0; i < carpcont; i++) {
        char *dir = carp[i], **files;
        int count, j;

        files = listarDir(dir, &count);
        if (files == NULL) {
            continue;
        }
        ordenar(files, count);
        if (chdir(dir) != 0) {
            error("mils: no se puede cambiar de directorio");
            goto cleanup;
        }
        if (carpcont > 0 && token.count > 1) {
            fprintf(stdout, "\n%s:\n", dir);
        }
        leerFiles(files, count);
        if (chdir(cwd) != 0) {
            error("mils: no se puede cambiar de directorio");
            exit(1);
        }
        
        cleanup:
        for (j = 0; j < count; j++) {
            free(files[j]);
        }
        free(files);
    }
    free(cwd);
}

int main(int argc, char **argv) {
    char **carp, **files;
    int count, carpcont;

    if (leerTokens(argc, argv) != 0) {
        fprintf(stdout,
            "Uso: ./micp -[al] [Archivo || Carpeta]\n"
            "Lista informacion sobre los archivos(directorio actual por defecto).\n"
            "Los ordena alfabeticamente\n"
            "-a, --all          No ignora los archivos con . o ..\n"
            "-l, --listar       listar mostrando informacion\n"
            "--help             informacion\n"
            );
        return 0;
    }


    carp = malloc(sizeof(char*) * token.count);
    files = malloc(sizeof(char*) * token.count);
    if (carp == NULL || files == NULL) {
        free(carp);
        free(files);
        return 1;
    }
    
    clasifica(carp, &carpcont, files, &count);

    ordenar(files, count);
    ordenar(carp, carpcont);
    leerFiles(files, count);
    leerDir(carp, carpcont);

    free(carp);
    free(files);
    return EXIT_SUCCESS;
}
