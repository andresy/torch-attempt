#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "generic/THStorage.h"
#else

/* ils appellent tous la vtable... THStorage est un module apres tout.
   donc newMappedFile est dans le module. voila.
   et les autres servent pour le polymorphisme.
   donc en cas de sous classe (genre MappedStorage! on peut toujours utiliser ca)
   c'est juste que le module sous-classe sera d'un nom different. logique. */

typedef struct THStorage THStorage;

THStorage* THStorage_(new)(void);

THStorage* THStorage_(newWithSize)(long);
THStorage* THStorage_(newWithSize1)(real);
THStorage* THStorage_(newWithSize2)(real, real);
THStorage* THStorage_(newWithSize3)(real, real, real);
THStorage* THStorage_(newWithSize4)(real, real, real, real);

THStorage* THStorage_(newWithData)(real *data);

THStorage* THStorage_(newWithMapping)(const char*, int);

real* THStorage_(data)(THStorage*);
long THStorage_(size)(THStorage*);

void THStorage_(fill)(THStorage*, real);

void THStorage_(retain)(THStorage*);
void THStorage_(copy)(THStorage*, THStorage*);
void THStorage_(resize)(THStorage*, long);

real THStorage_(get)(THStorage*, long);
void THStorage_(set)(THStorage*, long, real);

void THStorage_(free)(THStorage*);

#endif
