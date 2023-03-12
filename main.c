/* STEFAN Alexia - 312 CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nod {
  char *numefisier;
  struct nodd *parent;
  struct nod *st, *dr;
} nod; // structura pt FISIERE

typedef struct nodd {
  char *numedir;
  struct nodd *parent;
  struct nod *files;
  struct nodd *subdir;
  struct nodd *st, *dr;

} nodd; // structura pt FOLDERE

struct nod *search_fis(struct nod *a, char *x) {

  if (a == NULL)
    return NULL;
  if (strcmp(a->numefisier, x) == 0)
    return a;
  if (strcmp(x, a->numefisier) > 0)
    return search_fis(a->dr, x);
  else
    return search_fis(a->st, x);
}

struct nodd *search_dir(struct nodd *a, char *nume) {

  if (a == NULL)
    return NULL;
  if (strcmp(a->numedir, nume) == 0)
    return a;
  if (strcmp(nume, a->numedir) > 0)
    return search_dir(a->dr, nume);
  else
    return search_dir(a->st, nume);
}

void touch(char *nume, struct nod **a, struct nodd *p) {
  if (*a == NULL) {
    *a = malloc(sizeof(struct nod));
    if (*a == NULL)
      return;
    (*a)->numefisier = malloc((strlen(nume) + 1) * sizeof(char));
    if ((*a)->numefisier == NULL) {
      free(*a);
      return;
    }
    strcpy((*a)->numefisier, nume);
    (*a)->parent = p;
    (*a)->st = NULL;
    (*a)->dr = NULL;
  } else {
    if (strcmp(nume, (*a)->numefisier) > 0) {
      touch(nume, &((*a)->dr), p);
    } else {
      touch(nume, &((*a)->st), p);
    }
  }
}

void ls_dir(struct nodd *a) {
  if (a == NULL)
    return;
  ls_dir(a->st);
  printf("%s ", a->numedir);
  ls_dir(a->dr);
}

void ls_fis(struct nod *a) {
  if (a == NULL)
    return;
  ls_fis(a->st);
  printf("%s ", a->numefisier);
  ls_fis(a->dr);
}

void ls(struct nodd *a) {
  ls_dir(a->subdir);
  ls_fis(a->files);
  printf("\n");
}

void mkdir(char *nume, struct nodd **a, struct nodd *p) {
  if (*a == NULL) {
    *a = malloc(sizeof(struct nodd));
    if (*a == NULL)
      return;
    (*a)->numedir = malloc((strlen(nume) + 1) * sizeof(char));
    if ((*a)->numedir == NULL) {
      free(*a);
      return;
    }
    strcpy((*a)->numedir, nume);
    (*a)->parent = p;
    (*a)->st = NULL;
    (*a)->dr = NULL;
    (*a)->files = NULL;
    (*a)->subdir = NULL;
  } else {
    if (strcmp(nume, (*a)->numedir) > 0) {
      mkdir(nume, &((*a)->dr), p);
    } else {
      mkdir(nume, &((*a)->st), p);
    }
  }
}

void rm(struct nod **a, char *nume) {
  if (strcmp((*a)->numefisier, nume) < 0)
    rm(&((*a)->dr), nume);
  else if (strcmp((*a)->numefisier, nume) > 0)
    rm(&((*a)->st), nume);
  else {
    if ((*a)->st == NULL) {
      (*a) = (*a)->dr;
      return;
    }
    if ((*a)->dr == NULL) {
      (*a) = (*a)->st;
      return;
    }
    struct nod *p = (*a)->dr;
    struct nod *q = NULL;

    while (p->st != NULL) {
      q = p;
      p = p->st;
    }
    if (q != NULL) {
      q->st = p->dr;
      p->dr = (*a)->dr;
    }

    p->st = (*a)->st;
    q = (*a);
    (*a) = p; // inlocuiesc cu minimul gasit
    free(q->numefisier);
    free(q);
  }
}

void rmdir(struct nodd **a, char *nume) {
  if (strcmp((*a)->numedir, nume) < 0)
    rmdir(&((*a)->dr), nume);
  else if (strcmp((*a)->numedir, nume) > 0)
    rmdir(&((*a)->st), nume);
  else {
    if ((*a)->st == NULL) {
      (*a) = (*a)->dr;
      return;
    }
    if ((*a)->dr == NULL) {
      (*a) = (*a)->st;
      return;
    }
    struct nodd *p = (*a)->dr;
    struct nodd *q = NULL;

    while (p->st != NULL) {
      q = p;
      p = p->st;
    }
    if (q != NULL) {
      q->st = p->dr;
      p->dr = (*a)->dr;
    }

    p->st = (*a)->st;
    q = (*a);
    (*a) = p; // inlocuiesc cu minimul gasit
    free(q->numedir);
    while ((q->files) != NULL)
      rm(&(q->files), q->files->numefisier);

    while ((q->subdir) != NULL)
      rmdir(&(q->subdir), q->subdir->numedir);

    free(q);
  }
}

struct nodd *cd(struct nodd *a, char *x) {

  if (strcmp(x, "..") == 0) {
    if ((a)->parent != NULL)
      return (a)->parent;

    return NULL;
  }
  if (search_dir((a)->subdir, x) == NULL)
    return a;

  return search_dir((a)->subdir, x);
}

void pwd(struct nodd *a) {

  if (a != NULL) {
    pwd(a->parent);
    printf("/%s", a->numedir);
  } else
    return;
}

struct nodd *find_d(struct nodd *a, char *nume) {
  if (a != NULL) {

    struct nodd *pd = search_dir(a, nume);

    if (pd != NULL) {

      return pd;
    }

    struct nodd *p = find_d(a->st, nume);

    if (p != NULL) {
      return p;
    }

    p = find_d(a->dr, nume);
    if (p != NULL) {
      return p;
    }

    p = find_d(a->subdir, nume);
    if (p != NULL) {
      return p;
    }
  }
  return NULL;
}

struct nodd *find_f(struct nodd *a, char *nume) {
  if (a != NULL) {

    struct nod *pf = search_fis(a->files, nume);

    if (pf != NULL) {
      return pf->parent;
    }

    struct nodd *p = find_f(a->st, nume);
    if (p != NULL) {
      return p;
    }

    p = find_f(a->dr, nume);
    if (p != NULL) {
      return p;
    }
    p = find_f(a->subdir, nume);
    if (p != NULL) {
      return p;
    }
  }
  return NULL;
}

int main() {

  struct nodd *ROOT;
  ROOT = malloc(sizeof(struct nodd));
  if (ROOT == NULL) {
    return 0;
  }
  ROOT->numedir = malloc(sizeof(char) * (strlen("root") + 1));
  if (ROOT->numedir == NULL) {
    free(ROOT);
    return 0;
  }

  strcpy(ROOT->numedir, "root");
  ROOT->parent = NULL;
  ROOT->subdir = NULL;
  ROOT->files = NULL;
  ROOT->st = NULL;
  ROOT->dr = NULL;

  size_t lenline = 70;
  char *delimitator;
  char *input = (char *)malloc(lenline * sizeof(char));
  if (input == NULL) {
    free(ROOT->numedir);
    free(ROOT);
    return 0;
  }
  struct nodd *curent;
  curent = ROOT;

  while (getline(&input, &lenline, stdin) != 0) {

    delimitator = strtok(input, " \n");

    if (strcmp(delimitator, "touch") == 0) {
      delimitator = strtok(NULL, " \n");
      if (search_fis(curent->files, delimitator) != NULL)
        printf("File %s already exists!\n", delimitator);
      else if (search_dir(curent->subdir, delimitator) != NULL)
        printf("Directory %s already exists!\n", delimitator);
      else
        touch(delimitator, &curent->files, curent);
    } else if (strcmp(delimitator, "mkdir") == 0) {

      delimitator = strtok(NULL, " \n");
      if (search_fis(curent->files, delimitator) != NULL)
        printf("File %s already exists!\n", delimitator);
      else if (search_dir(curent->subdir, delimitator) != NULL)
        printf("Directory %s already exists!\n", delimitator);
      else
        mkdir(delimitator, &curent->subdir, curent);
    } else if (strcmp(delimitator, "ls") == 0) {
      ls(curent);
    } else if (strcmp(delimitator, "rm") == 0) {
      delimitator = strtok(NULL, " \n");
      if (search_fis(curent->files, delimitator) == NULL)
        printf("File %s doesn't exist!\n", delimitator);
      else
        rm(&(curent->files), delimitator);
    } else if (strcmp(delimitator, "rmdir") == 0) {
      delimitator = strtok(NULL, " \n");
      if (search_dir(curent->subdir, delimitator) == NULL)
        printf("Directory %s doesn't exist!\n", delimitator);
      else
        rmdir(&(curent->subdir), delimitator);
    } else if (strcmp(delimitator, "cd") == 0) {
      delimitator = strtok(NULL, " \n");

      if (cd(curent, delimitator) == curent)
        printf("Directory not found!\n");
      else
        curent = cd(curent, delimitator);
    } else if (strcmp(delimitator, "pwd") == 0) {
      pwd(curent);
      printf("\n");
    } else if (strcmp(delimitator, "find") == 0) {
      delimitator = strtok(NULL, " -\n");

      if (strcmp(delimitator, "f") == 0) {

        delimitator = strtok(NULL, " -\n");
        if (find_f(ROOT, delimitator) != NULL) {
          printf("File %s found!\n", delimitator);
          pwd(find_f(ROOT, delimitator));
          printf("\n");
        } else if (find_f(ROOT, delimitator) == NULL) {
          printf("File %s not found!\n", delimitator);
        }
      }

      if (strcmp(delimitator, "d") == 0) {
        delimitator = strtok(NULL, " -\n");
        if (find_d(ROOT, delimitator) != NULL) {
          printf("Directory %s found!\n", delimitator);
          pwd(find_d(ROOT, delimitator));
          printf("\n");
        } else if (find_d(ROOT, delimitator) == NULL) {
          printf("Directory %s not found!\n", delimitator);
        }
      }
    }
    if (strcmp(delimitator, "quit") == 0) {
      break;
    }
  }
  free(ROOT->numedir);
  free(ROOT);
  free(input);

  return 0;
}
