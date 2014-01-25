#include "Auxiliary.h"
#include "Logging.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>

//#include <io.h> //KNOTE: commented by Kirill

namespace Aux
{
    const int TIME_STRING_LENGTH = 32;

    //уравнение dT=a0 + a1*index_1 + a2 * index_2
    void MNK(const double *index_1,
        const double *index_2,
        const double *values,
        const int n, //length of index_1
        const int m, //length of index_2
        double& a0,
        double& a1,
        double& a2)
    {
        const int dim = 3;//task dimension

        double *A = new double [n * m * dim];

        //иходная матрица A = [1,index_1,index_2] , (n*m строк)
        for(int i1 = 0 ; i1 < n ; i1++)
            for(int j1 = 0 ; j1 < m ; j1++)
            {
                A[(j1 + m * i1) * dim + 0] = 1;
                A[(j1 + m * i1) * dim + 1] = index_1[i1];
                A[(j1 + m * i1) * dim + 2] = index_2[j1];
            };

        double *M = new double [dim * dim];

        //M=0
        for(int i3 = 0 ; i3 < dim * dim ; i3++)
            M[i3] = 0.0;

        //M = transp(A) * A
        for (int i = 0 ; i < dim ; i++)
            for (int j = 0 ; j < dim ; j++)
                for (int k = 0 ; k < m * n ; k++)
                    M[i * dim + j] += A[k * dim + i] * A[k * dim + j];

        double *Y = new double [dim];

        //Y = 0
        for(int i2 = 0; i2 < dim; i2++)
            Y[i2] = 0.0;

        //Y = transp(A) * values
        for(int h = 0 ; h < dim ; h++)
            for(int g = 0 ; g < m * n ; g++)
                Y[h] += A[g * dim + h] * values[g];

        //detM=||M||
        double detM = (M[0*3+1] * M[2*3+0] * M[1*3+2] -
            M[0*3+1] * M[1*3+0] * M[2*3+2] +
            M[1*3+0] * M[2*3+1] * M[0*3+2] +
            M[1*3+1] * M[0*3+0] * M[2*3+2] -
            M[2*3+0] * M[1*3+1] * M[0*3+2] -
            M[2*3+1] * M[0*3+0] * M[1*3+2]);

        //calculate T and R
        a0 = (M[1*3+1] * Y[0] * M[2*3+2]
        - M[1*3+1] * M[0*3+2] * Y[2]
        + M[0*3+2] * M[2*3+1] * Y[1]
        - Y[0] * M[2*3+1] * M[1*3+2]
        + M[0*3+1] * M[1*3+2] * Y[2]
        - M[0*3+1] * Y[1] * M[2*3+2]) / detM;


        a1 = - (-Y[0] * M[2*3+0] * M[1*3+2]
        +	Y[0] * M[1*3+0] * M[2*3+2]
        +	M[0*3+0] * M[1*3+2] * Y[2]
        -	M[0*3+0] * Y[1] * M[2*3+2]
        -	M[0*3+2] * M[1*3+0] * Y[2]
        +	M[0*3+2] * M[2*3+0] * Y[1]) / detM;

        a2 = (-M[0*3+1] * M[1*3+0] * Y[2]
        - M[2*3+1] * M[0*3+0] * Y[1]
        + M[1*3+0] * M[2*3+1] * Y[0]
        +	M[0*3+1] * M[2*3+0] * Y[1]
        + M[1*3+1] * M[0*3+0] * Y[2]
        - M[2*3+0] * M[1*3+1] * Y[0]) / detM;

        delete [] Y;
        delete [] M;
        delete [] A;
    }

    //NOTE: stolen from alglib-2.6.0.cpp.zip 
    double correlation(double* x, double* y, int n)
    {
        double result;
        int i;
        double xmean;
        double ymean;
        double s;
        double xv;
        double yv;
        double t1;
        double t2;

        xv = 0;
        yv = 0;
        if( n<=1 )
        {
            result = 0;
            return result;
        }

        //
        // Mean
        //
        xmean = 0;
        ymean = 0;
        for(i = 0; i <= n-1; i++)
        {
            xmean = xmean+x[i];
            ymean = ymean+y[i];
        }
        xmean = xmean/n;
        ymean = ymean/n;

        //
        // numerator and denominator
        //
        s = 0;
        xv = 0;
        yv = 0;
        for(i = 0; i <= n-1; i++)
        {
            t1 = x[i]-xmean;
            t2 = y[i]-ymean;
            xv = xv + t1*t1;
            yv = yv + t2*t2;
            s = s+t1*t2;
        }
        double epsilon = 1.0e-100;
        if( fabs(xv) < epsilon || fabs(yv) < epsilon )
        {
            result = 0;
        }
        else
        {
            result = s/(sqrt(xv)*sqrt(yv));
        }
        return result;
    }

    void GetApplicationTimeStamp(char* timeString)
    {
        time_t ltime = Logger::GetStartTime();
        strftime(timeString, TIME_STRING_LENGTH, "20%y-%m-%d_%H-%M-%S", localtime(&ltime));
    }

    string GetUniqueName(string dirName, char* timeStamp, string fileNameBase, string extension)
    {
        string uniqueName = dirName + timeStamp +  "_" + fileNameBase + extension;

        //duplicates checking optimization
        static string previousUniqueFileName;
        static int nFilesExist = 0;
        if (previousUniqueFileName != uniqueName)
        {
            nFilesExist = 0;
            previousUniqueFileName = uniqueName;
        }

        //check duplicates and generate unique name
        char buf[10];
        FILE* f;
        int lenFileName = uniqueName.length();
        for (; (f = fopen(uniqueName.c_str(), "r")) != NULL; fclose(f))
        {
            if (nFilesExist < 9)
                sprintf(buf, "0%d", ++nFilesExist);
            else
                sprintf(buf, "%d", ++nFilesExist);
            uniqueName = uniqueName.substr(0, lenFileName - extension.length()) + "_" + buf +  extension;
        }

        return uniqueName;
    }

    string CreateCoolFileName(string dirName, string fileNameBase, string extension)
    {
        //Директория должна заканчиваться \\
        //папки будут создаваться в папке bin
        //Имя не содержит директории и не содержит расширения
        //Расширение обязательно

        char timeStamp[TIME_STRING_LENGTH];
        GetApplicationTimeStamp(timeStamp);

        if ((extension[0] != '.') && (extension != ""))
            extension = "." + extension;

        char deniedSymbols[] = "./\\:*?\"<>|";
        for (unsigned int i = 0; i < strlen(deniedSymbols); ++i)
            replace(fileNameBase.begin(), fileNameBase.end(), deniedSymbols[i],'_');

        for (unsigned int i = 0; i < dirName.length(); i++)
            if (dirName[i] == '\\')
            {
                //FIXME: implement cross-platform directory creation
                //CreateDirectory(dirName.substr(0, i).c_str(), NULL);
            }

        return GetUniqueName(dirName, timeStamp, fileNameBase, extension);
    }

    string IntToString(int intValue, int length)
    { 
        char myBuff[256];
        memset(myBuff, '\0', 256); 
        //itoa(intValue, myBuff, 10);
        sprintf(myBuff, "%d", intValue);

        string strRetVal = myBuff;
        while (strRetVal.size() < length)
        {
            string zero = string("0");
            zero += strRetVal;
            strRetVal = zero;
        }

        return(strRetVal); 
    }

    string ChangeExtention(const string& filename, const char* newExt)
    {
        int pos = (int)filename.length() - 1;
        while(pos >= 0 && filename[pos] != '.')
        {
            if (filename[pos] == '\\' || filename[pos] == '/')
            {
                pos = (int)filename.length();
                break;
            }
            pos--;
        }
        string result(filename);
        if (pos < result.length())
            result.replace(pos + 1, result.length() - pos - 1, newExt[0] == '.' ? newExt + 1 : newExt);
        else
            if (newExt[0] == '.')
                result.append(newExt);
            else
            {
                result.append(".", 1);
                result.append(newExt);
            }
            return result;
    }

    bool FileExists(const string& strFilename)
    {
        struct stat stFileInfo;
        bool blnReturn;
        int intStat;

        // Attempt to get the file attributes
        intStat = stat(strFilename.c_str(),&stFileInfo);
        if(intStat == 0) {
            // We were able to get the file attributes
            // so the file obviously exists.
            blnReturn = true;
        } else {
            // We were not able to get the file attributes.
            // This may mean that we don't have permission to
            // access the folder which contains this file. If you
            // need to do that level of checking, lookup the
            // return values of stat which will give you
            // more details on why stat failed.
            blnReturn = false;
        }
        return(blnReturn);
    }

    unsigned GetFileSize(const string& strFilename)
    {
        struct stat stFileInfo;
        int intStat = stat(strFilename.c_str(), &stFileInfo);
        if (intStat == 0)
            return (unsigned)stFileInfo.st_size;
        return 0;
    }

    string Format(const char* format, ...)
    {   
        //TODO: hardcoded 256 may be bad
//        va_list  argList;
//        va_start(argList, format);

//        int len = vsnprintf(0, 0, format, argList);
//        string result(len+1, 0);
//        vsnprintf((char*)result.c_str(), len + 1, format, argList);

//        va_end(argList);

//        return result;

        char buff[256];

        va_list argList;
        va_start(argList, format);
        vsnprintf(buff, 256, format, argList);
        va_end(argList);

        string result(buff);
        return result;
    }
}
