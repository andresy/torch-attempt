template <typename T> class Storage
{
  public:
    T *data;
    long size;
    long refcount;
    
    Storage();
    Storage(long size);

    void retain();
    void free();
    void resize(long size_, bool keepContent_);
    void copy(Storage<T> *src);

    void copyChar(Storage<char> *src);
    void copyShort(Storage<short> *src);
    void copyInt(Storage<int> *src);
    void copyLong(Storage<long> *src);
    void copyFloat(Storage<float> *src);
    void copyDouble(Storage<double> *src);

    ~Storage();
};
