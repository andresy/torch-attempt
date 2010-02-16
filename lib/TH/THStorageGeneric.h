#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "THStorageGeneric.h"
#else

/* ils appellent tous la vtable... THStorage est un module apres tout.
   donc newMappedFile est dans le module. voila.
   et les autres servent pour le polymorphisme.
   donc en cas de sous classe (genre MappedStorage! on peut toujours utiliser ca)
   c'est juste que le module sous-classe sera d'un nom different. logique. */

typedef struct THStorage THStorage;

THStorage* THStorage_(alloc)();

THStorage* THStorage_(allocWithSize)(long);
THStorage* THStorage_(allocMappedFile)(const char*, int);

real* THStorage_(data)(THStorage*);
long THStorage_(size)(THStorage*);

void THStorage_(retain)(THStorage*);
void THStorage_(copy)(THStorage*, THStorage*);
void THStorage_(resize)(THStorage*, long);

void THStorage_(free)(THStorage*);

#endif
