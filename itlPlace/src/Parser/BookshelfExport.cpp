#include "BookshelfExport.h"
#include <iostream>//неободимо для ввода вывода
#include <time.h>//необходимо для определения времени
#include "Configuration.h"
#include "CellUtils.h"
//string GenerateFileName()//добавляет в название pl файла название программы и время через подчеркивание
//{	 
//	time_t ltime;//время 
//	time( &ltime );
//	string fileName;//возвращаемая строка
//	char newFileName[256];//временная строка
//	char *pVal;
//	int nBinRows = 1, nBinCols = 1;
//
//	if ( std::string( cfg.lookup( "benchmark.def" )).c_str() != 0 )
//	  strcpy(newFileName,  std::string( cfg.lookup( "benchmark.def" )).c_str())	 ;//берем имя из конфига
//	
//	pVal = strrchr(newFileName, '\\');
//	
//	if ( pVal ) 
//		strcpy(newFileName,  ++pVal);
//	
//	pVal = strrchr(newFileName , '.');//удаляем расширение файла
//	
//	if ( pVal ) 
//		pVal[0] = '\0';
//	
//	strcat(newFileName, "itlPlace ");//пишем название программы
//	strcat(newFileName, ctime( &ltime ));//пишем время
//	pVal = strrchr(newFileName, '\n');
//	
//	if ( pVal ) 
//		pVal[0] = '\0'; 
//  
//	strcat(newFileName, ".pl");//добавляем нужное расширение файла
//  
//	while ( 1 )//заменяем пробел подчеркиванием
//	{ 
//		pVal = strchr(newFileName, ' ');
//		if ( pVal ) 
//		  pVal[0] = '_';
//		else 
//			break;
//	}
//	
//	while ( 1 )
//	{ 
//		pVal = strchr(newFileName, ':');//заменяем : подчеркиванием
//		if( pVal ) 
//			pVal[0] = '_';
//		else 
//			break;
//	};
//	
//	fileName = newFileName;
//	return fileName;//выводим
//}//GenerateFileName()
//
//
//string getStringOrientation(CellOrientation i_Orientation)//возвращает строковое значение орентации
//{
//	std::string sOrientation = "N";
//		
//	if ( i_Orientation == 0 ) //определяем ориентацию фейла
//	  sOrientation = "N";
//	
//	if ( i_Orientation == 1 ) 
//		sOrientation = "W";
//	
//	if ( i_Orientation == 2 ) 
//	  sOrientation = "S";
//  
//	if ( i_Orientation == 3 ) 
//		sOrientation = "E";
//  
//	if ( i_Orientation == 4 ) 
//		sOrientation = "FN";
//  
//	if ( i_Orientation == 5 ) 
//		sOrientation = "FW";
//  
//	if ( i_Orientation == 6 ) 
//		sOrientation = "FS";
//  
//	if ( i_Orientation == 7 ) 
//		sOrientation = "FE";
//		 
//	return sOrientation;
//};//getStringOrientation

void PrintToPL( HDesign& hd, const std::string fileName )//печать в фейл.pl
{  


	 time_t ltime;//время
	 time( &ltime );
	 FILE *dataFile;
 
	 int nBinRows = 1, nBinCols = 1;
	 
	 printf( "Dumping placement to \" %s \"", fileName.c_str());

	if (( dataFile = fopen(fileName.c_str(), "w")) == 0)//открылся ли файл
  {
    printf( "ERROR: could not open output pl file. %s \n", fileName.c_str());
    return ; ///TODO: think about error codes
  }
		
	 //f = fopen(newFileName, "w");
	 fprintf( dataFile , "UCLA pl 1.0\n");
	 fprintf( dataFile , "# ITLab\n# Created : %s" , ctime( &ltime ));
   fprintf( dataFile , "# Work time: %d\n", 0);
   fprintf( dataFile , "# Wire length: %d\n", 0);
   fprintf( dataFile , "#_Bin grid: %d x %d\n\n" , nBinRows, nBinCols);
	 
	 for(HCells::CellsEnumeratorW i = hd.Cells.GetEnumeratorW() ; i.MoveNext(); )//цикл печати элементов
	 {
     if (i.PlacementStatus() != PlacementStatus_Movable
       && i.PlacementStatus() != PlacementStatus_Fixed)
       continue;

		 if ( ferror(dataFile ) != 0) //проверка на ошибку
		 {
       printf( "ERROR: %d At a seal in a file %s \n", ferror(dataFile), fileName.c_str());
			 return;
		 }
	
		 //печатаем в файл имя, x, y, ориентация
		 fprintf(dataFile,
       "%8s %10.3f %10.3f : %s\n", 
       i.Name().c_str(),
       i.X(),
       i.Y(),
       Utils::GetCellOrientStr(i.Orientation())
       );
	 }
	 
	 fclose( dataFile );//закрываем файл
}
