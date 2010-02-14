/* ils appellent tous la vtable... THStorage est un module apres tout.
   donc newMappedFile est dans le module. voila.
   et les autres servent pour le polymorphisme.
   donc en cas de sous classe (genre MappedStorage! on peut toujours utiliser ca)
   c'est juste que le module sous-classe sera d'un nom different. logique. */

typedef struct THStorage THStorage;

extern const struct THStorageAPI
{
  THStorage* (*alloc)();

  THStorage* (*allocWithSize)(long);
  THStorage* (*allocMappedFile)(const char*, int);
  
  real* (*data)(THStorage*);
  long (*size)(THStorage*);

  void (*retain)(THStorage*);
  void (*copy)(THStorage*, THStorage*);
  void (*resize)(THStorage*, long);
  
  void (*free)(THStorage*);
  
} THStorageAPI;

