#define WRITE(var, type, count) file.write((char*)var,sizeof(type)*count)
#define WRITEC(var, count) file.write((char*)var,sizeof(unsigned char)*count)
#define WRITEI(var) file.write((char*)var,sizeof(int))
#define WRITEB(var) file.write((char*)var,sizeof(bool))

#define READ(var, type, count) file.read((char*)var,sizeof(type)*count)
#define READC(var, count) file.read((char*)var,sizeof(unsigned char)*count)
#define READI(var) file.read((char*)var,sizeof(int))
#define READB(var) file.read((char*)var,sizeof(bool))
