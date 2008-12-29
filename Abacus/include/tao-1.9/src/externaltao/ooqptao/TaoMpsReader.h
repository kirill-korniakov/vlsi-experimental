#ifndef TAOMPSREADER
#define TAOMPSREADER

class TaoVec;
class TaoMat;

#ifdef __cplusplus
extern "C" {
#endif
  int newTaoMpsReader( char filename[], void ** reader );
  
  void TaoMpsReaderGetSizes( void * reader_, int * nx, int * ny, int * nz );

  void TaoMpsReaderGetNNZ( void * reader_,
			   int * nnzQ, int * nnzA, int * nnzC );

  int freeTaoMpsReader( void ** reader );

  int  TaoMpsReaderReadQpGen( void * reader, double * f,
			      TaoVec * c, TaoMat * Q,
			      TaoVec *  xlow, TaoVec *  xupp, 
			      TaoMat *  A,     TaoVec * b,
			      TaoMat *  C,
			      TaoVec *  clow,  TaoVec *  cupp );

#ifdef __cplusplus
};
#endif

#endif
