#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//14byte
typedef struct
{
  char cfType[2];         /*  "BM" (0x4D42)*/
  char cfSize[4];         
  char cfReserved[4];     
  char cfoffBits[4];      
}__attribute__((packed)) BITMAPFILEHEADER;  

    
//40byte
typedef struct
{
  char ciSize[4];         /* size of BITMAPINFOHEADER */
  char ciWidth[4];        
  char ciHeight[4];       
  char ciPlanes[2];       
  char ciBitCount[2];     
  char ciCompress[4];     
  char ciSizeImage[4];   
  char ciXPelsPerMeter[4];
  char ciYPelsPerMeter[4];
  char ciClrUsed[4];      
  char ciClrImportant[4]; 
}__attribute__((packed)) BITMAPINFOHEADER;       




BITMAPFILEHEADER FileHead;
BITMAPINFOHEADER InfoHead;



int  show_bmp  ( char *bmpfile );
long chartolong ( char * string, int length );





int main ( int argc, char *argv[] )
{

	printf("sizeof header=%d\n", sizeof(BITMAPFILEHEADER));
	printf("into show_bmp fun using this %s file\n",__FILE__);

	(argc ==2)?show_bmp( argv[1] ):show_bmp("lena.bmp");

    return 0;
}


int show_bmp( char *bmpfile )
{
        FILE *fp;
	      FILE *newfp;
        newfp=fopen("negate.bmp","wb");
        unsigned char *Datas;
		
        int rc,b;
        int ciBitCount, ciWidth, ciHeight,cfOffset;   
       
        if ((fp = fopen( bmpfile, "rb" ))==NULL)
        {
                printf("error reading file!\n");
                return( -1 );
        }
        
       
        rc = fread( &FileHead, sizeof(BITMAPFILEHEADER),1, fp );
        if ( rc != 1)
        {
		            printf("read header error!\n");
                fclose( fp );
                return( -2 );
        }     

       
        rc = fread( (char *)&InfoHead, sizeof(BITMAPINFOHEADER),1, fp );
        if ( rc != 1)
        {
		            printf("read infoheader error!\n");
                fclose( fp );
                return( -4 );
        }

        ciWidth    = (int) chartolong( InfoHead.ciWidth,    4 );
        ciHeight   = (int) chartolong( InfoHead.ciHeight,   4 );
        ciBitCount = (int) chartolong( InfoHead.ciBitCount, 4 );
        cfOffset  =  (int) chartolong(FileHead.cfoffBits, 4);

    	

            
    	printf("width=%d, height=%d, bitCount=%d, offset=%d\n",ciWidth,ciHeight,ciBitCount,cfOffset);
    	

      fwrite(&FileHead, sizeof(BITMAPFILEHEADER),1, newfp );
      fwrite(&InfoHead, sizeof(BITMAPINFOHEADER),1, newfp );

      

      fseek(fp, (int)chartolong(FileHead.cfoffBits, 4), SEEK_SET);


      Datas=(char*) calloc(((ciWidth)*(ciHeight)*3), sizeof(unsigned char));
     	fread(Datas, sizeof(unsigned char), (ciWidth)*(ciHeight)*3, fp);


      int i=0,s;
      while (i<(ciWidth)*(ciHeight)*3){

		*(Datas+i)=255-*(Datas+i);
		  
          i++;       

      }


      fwrite(Datas, sizeof(unsigned char),(ciWidth)*(ciHeight)*3, newfp );

      
    	free (Datas);
    	fclose( fp );
    	fclose(newfp);
        return( 0 );
}



long chartolong( char * string, int length )
{
        long number;
        
        if (length <= 4)
        {
                memset( &number, 0x00, sizeof(long) );
                memcpy( &number, string, length );
        }
        
        return( number );
}

