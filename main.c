//this is just addded 
#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <assert.h>

//14byte
typedef struct
{
  char cfType[2];         /*  "BM" (0x4D42)*/
  char cfSize[4];
  char cfReserved[4];
  char cfoffBits[4];
}__attribute__((packed)) myBITMAPFILEHEADER;


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
}__attribute__((packed)) myBITMAPINFOHEADER;




myBITMAPFILEHEADER FileHead;
myBITMAPINFOHEADER InfoHead;
unsigned char *Datas;
FILE *fp;
int ciBitCount, ciWidth, ciHeight,cfOffset,cisize;

int  show_bmp  ( const char *bmpfile );
long chartolong ( char * string, int length );

int i;
void negative(void){
for(i=0;i<(ciWidth)*(ciHeight)*ciBitCount/8;i++){
          *(Datas+i)=255-*(Datas+i);

      }
return;

}

void reload(void){
free(Datas);
Datas=(unsigned char*) calloc(((ciWidth)*(ciHeight)*ciBitCount/8), sizeof(unsigned char));
fseek(fp, cfOffset, SEEK_SET);
fread(Datas, sizeof(unsigned char), (ciWidth)*(ciHeight)*ciBitCount/8, fp);
return;

}

void bright(void){
int i=0,s;
      while (i<(ciWidth)*(ciHeight)*ciBitCount/8){
          s=*(Datas+i)+100;
	  if (s>=255)
		*(Datas+i)=255;
	  else if (s<=0)
		*(Datas+i)=0;
	  else
	  {
	  	*(Datas+i)=s;
	  }
          i++;

      }

}


void mylog(void){
      int i=0,logd;
      while (i<(ciWidth)*(ciHeight)*ciBitCount/8){
          logd=100*log(1+*(Datas+i));

	  if (logd>=255)
		*(Datas+i)=255;
	  if (logd<=0)
		*(Datas+i)=0;
	else
		*(Datas+i)=logd;
          i++;

      }

}


void thresh(void){

      int i=0;
      while (i<(ciWidth)*(ciHeight)*ciBitCount/8){
          if (*(Datas+i)>100)
		*(Datas+i)=255;
	else  *(Datas+i)=0;

	  if (*(Datas+i)>=255)
		*(Datas+i)=255;
	  if (*(Datas+i)<=0)
		*(Datas+i)=0;
          i++;

      }



}

void histogram(void){



      int i=0,j=0;
      int nk[256]={0};
float prk[256]={0.0};
float sk[256]={0.0};
int SK[256]={0};
	float n=1.0*ciWidth*ciWidth*ciBitCount/8;
      while (i<(ciWidth)*(ciHeight)*ciBitCount/8){
		nk[*(Datas+i)]++;
          i++;

      }

for (i=0;i<=255;i++){
prk[i]=nk[i]/n;
	}


	for (i=0;i<=255;i++){
	for (j=0;j<=i;j++)
	sk[i]+=prk[j];
	}


	for (i=0;i<=255;i++)
		SK[i]=255*sk[i];

      for (i=0;i<ciWidth*ciWidth*ciBitCount/8;i++){
		*(Datas+i) =SK[*(Datas+i)];

      }




}

int BITTOMASK=0;
void bitslice(void){

     int i=0;
      while (i<(ciWidth)*(ciHeight)*ciBitCount/8){
          if (*(Datas+i) & (1<<BITTOMASK))
		*(Datas+i)=255;
	else  *(Datas+i)=0;

          i++;

      }


}

static int window;
static int menu_id;
static int submenu_id,bit_id;
static int value = 0;


void convolve(int mat[]){


        unsigned char *Datas2d;
        Datas2d=(unsigned char*) calloc(((ciWidth)*(ciHeight)), sizeof(unsigned char));
        int i=1,j=1;
        float b;
      while (i<ciHeight-1){
          j=1;
          while(j<ciWidth-1){
	  b=*(Datas+(i-1)*ciHeight+j-1)*mat[0]+*(Datas+(i-1)*ciHeight+j)*mat[1]+*(Datas+(i-1)*ciHeight+j+1)*mat[2]+
        *(Datas+(i)*ciHeight+j-1)*mat[3]+ *(Datas+(i)*ciHeight+j)*mat[4]+ *(Datas+(i)*ciHeight+j+1)*mat[5]+
        *(Datas+(i+1)*ciHeight+j-1)*mat[7]+*(Datas+(i+1)*ciHeight+j)*mat[8]+*(Datas+(i+1)*ciHeight+j+1)*mat[8];

	  if (b>=255)
		*(Datas2d+i*512+j)=255;
	  if (b<=0)
		*(Datas2d+i*512+j)=0;
	  else{
        if (value==8)
		*(Datas2d+i*512+j)=(int)b/9;
		else
		*(Datas2d+i*512+j)=(int)b;
	  }
        j++;

      }

 i++;

      }


i=0;
      while (i<(ciWidth)*(ciHeight)){
 *(Datas+i)=*(Datas2d+i);
          i++;
      }

}


void median(void){

        unsigned char *Datas2d;
        Datas2d=(unsigned char*) calloc(((ciWidth)*(ciHeight)), sizeof(unsigned char));


    int mat[9];
    int i=1,j=1,l,m;
    unsigned char temp;
      while (i<ciHeight-1){
          j=1;
          while(j<ciWidth-1){

	  mat[0]=*(Datas+(i-1)*ciHeight+j-1);
	  mat[1]=*(Datas+(i-1)*ciHeight+j);
	  mat[2]=*(Datas+(i-1)*ciHeight+j+1);
	  mat[3]=*(Datas+(i)*ciHeight+j-1);
	  mat[4]=*(Datas+(i)*ciHeight+j);
	  mat[5]=*(Datas+(i)*ciHeight+j+1);
	  mat[6]=*(Datas+(i+1)*ciHeight+j-1);
	  mat[7]=*(Datas+(i+1)*ciHeight+j);
	  mat[8]=*(Datas+(i+1)*ciHeight+j+1);


	  for (l=0;l<9;l++){
	  for (m=0;m<8;m++){
	  if (mat[m]>mat[m+1])
	  temp=mat[m];
       mat[m]=mat[m+1];
       mat[m+1]=temp;
	  }}

		*(Datas2d+i*512+j)=mat[4];
        j++;
      }

 i++;
      }

    i=0;
      while (i<(ciWidth)*(ciHeight)){
 *(Datas+i)=*(Datas2d+i);
          i++;
      }

}




void derivative(void){

//int mat[9]={0,0,0,-1,0,1,0,0,0}; //conv mat //simple derivative
int mat[9]={-1,-1,-1,-1,8,-1,-1,-1,-1}; //conv mat //sharpening
convolve(mat);
}





void smooth(){
int mat[9]={1,1,1,1,1,1,1,1,1};
convolve(mat);
}



void sobel(){

int mat[9]={-1,-2,-1,0,0,0,1,2,1}; //conv mat
convolve(mat);

}



void prewitt(){

int mat[9]={-1,-1,-1,0,0,0,1,1,1}; //conv mat
convolve(mat);

}


int show_bmp( const char *bmpfile )
{

        int rc;
        if ((fp = fopen( bmpfile, "rb" ))==NULL)
        {
                printf("error reading file!\n");
                return( -1 );
        }


        rc = fread( &FileHead, sizeof(myBITMAPFILEHEADER),1, fp );
        if ( rc != 1)
        {
		            printf("read header error!\n");
                fclose( fp );
                return( -2 );
        }


        rc = fread( (char *)&InfoHead, sizeof(myBITMAPINFOHEADER),1, fp );
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
        cisize  =  (int) chartolong(InfoHead.ciSize, 4);



    printf("width=%d, height=%d, bitCount=%d, offset=%d\n size=%d\n",ciWidth,ciHeight,ciBitCount,cfOffset,cisize);


      fseek(fp, (int)chartolong(FileHead.cfoffBits, 4), SEEK_SET);
      Datas=(unsigned char*) calloc(ciWidth*ciHeight*ciBitCount/8, sizeof(unsigned char));
      fread(Datas, sizeof(unsigned char),ciWidth*ciHeight*ciBitCount/8, fp);

    	//fclose( fp );

        return 0;
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



void menu(int num){
  if(num == 0){
    glutDestroyWindow(window);
    exit(0);
  }else{
    value = num;
  }
  glutPostRedisplay();
}
void createMenu(void){
    bit_id = glutCreateMenu(menu);
    glutAddMenuEntry("bit0", 22);
    glutAddMenuEntry("bit1", 23);
    glutAddMenuEntry("bit2", 24);
    glutAddMenuEntry("bit3", 25);
    glutAddMenuEntry("bit4", 26);
    glutAddMenuEntry("bit5", 27);
    glutAddMenuEntry("bit6", 28);
    glutAddMenuEntry("bit7", 29);

    submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Negative", 2);
    glutAddMenuEntry("Derivative", 3);
    glutAddMenuEntry("Historgram equalize", 4);
    glutAddMenuEntry("Log", 5);
    glutAddMenuEntry("Bright", 6);
    glutAddMenuEntry("Threshold", 7);
    glutAddMenuEntry("Smooth", 8);
    glutAddMenuEntry("sobel", 10);
    glutAddMenuEntry("prewitt", 11);
    glutAddMenuEntry("Median", 12);
    glutAddSubMenu("Bitslice", bit_id);


    menu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Reload/clear", 1);
    glutAddSubMenu("Draw", submenu_id);
    glutAddMenuEntry("Quit", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void display(void){
  glClear(GL_COLOR_BUFFER_BIT);
if (value >=22 && value <=29){
BITTOMASK=value-22;
bitslice();
}
    if(value == 1){
        reload();

  }else if(value == 2){
      negative();

  }else if(value == 3){
      derivative();

  }else if(value == 4){
    histogram();
  }else if(value == 5){
      mylog();

  }else if(value == 6){
     bright();

  }else if(value == 7){
      thresh();

  }else if(value == 8){
      smooth();

  }else if(value == 10){
      sobel();

  }else if(value == 11){
      prewitt();

  }else if(value == 12){
      median();

  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (ciBitCount==8)
    glDrawPixels(ciWidth,ciHeight,GL_LUMINANCE,GL_UNSIGNED_BYTE, Datas);
  if (ciBitCount==24)
  glDrawPixels(ciWidth,ciHeight,GL_RGB,GL_UNSIGNED_BYTE, Datas);
    glutSwapBuffers();
  glFlush();
}



int main(int argc, char *argv[])
{

	printf("sizeof header=%d\n", sizeof(myBITMAPFILEHEADER));
	printf("into show_bmp fun using this %s file\n",__FILE__);
	show_bmp("lena.bmp");

    glutInit(&argc, argv);
    glutInitWindowSize(ciWidth, ciHeight);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    window=glutCreateWindow("GLUT Shapes");
    createMenu();
    glClearColor(0.0,0.0,0.0,0.0);
    glutDisplayFunc(display);
    glutMainLoop();
    free(Datas);
	printf("freed Datas\n");
	fclose(fp);
    return EXIT_SUCCESS;
}
