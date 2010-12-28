#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define  BUFFER_SIZE  70
#define  TRUE          1
#define  FALSE         0

int**  img;
int**  img2;
int**  img3;
int    numRows;
int    numCols;
int** temppicx; 
int** temppicy;
int**  sobelout;
int    maxVal;
FILE*  fo1;

void addtopixels(int** imgtemp, int value);
void  writeoutpic(char* fileName, int** imgtemp);
int** readpic(char* fileName);
void  readHeader(FILE* imgFin);
int   isComment(char* line);
void  readImgID(char* line);
void  readImgSize(char* line);
void  readMaxVal(char* line);
int** setImage();
void  readBinaryData(FILE* imgFin, int** imgtemp);
void subtract(int** pic, int** pic2, int** pic3);
void sobelfunc(int** pic, int** edges, int** tempx, int** tempy);


int main()
{
	char fileName[BUFFER_SIZE];
	char fileName2[BUFFER_SIZE];
	char fileName3[BUFFER_SIZE];
	int i,j,rows,cols, choice, val;
	char ci;
	
	printf("1.Change values of a picture \n2.Subtract pictures \n3.Sobel edge highlighter \n4.quit\n");
    scanf("%d", &choice);
    
    // Menu.
    while (choice != 4) {
		
		if (choice==1){
			
			
			printf("Enter image filename: ");
			scanf("%s", fileName);
			
			img = readpic(fileName);
			
			printf("Successfully read image file '%s'\n", fileName);
			
			printf("How much value would you like to add?");
			scanf("%d", &val);
			
			addtopixels(img,val);
			
			printf("Enter image filename for output: ");
			scanf("%s", fileName);
			
			writeoutpic(fileName,img);
			
			free(img);
			img = NULL;
		}
		
		if (choice==2){
			
			printf("Enter first image filename: ");
			scanf("%s", fileName);
			
			img= readpic(fileName);
			
			printf("Successfully read image file '%s'\n", fileName);
			
			printf("Enter second image filename: ");
			scanf("%s", fileName2);
			
			img2=readpic(fileName2);
			
			printf("Successfully read image file '%s'\n", fileName2);
			
			img3= setImage();
			subtract(img,img2,img3);
			
			printf("Enter image filename for output: ");
			scanf("%s", fileName3);
			
			writeoutpic(fileName3,img3);
			
			free(img3);
			img=NULL;
			img2=NULL;
			img3=NULL;
		}
		
		if (choice==3){
			
			printf("Enter image filename: ");
			scanf("%s", fileName);
			
			img = readpic(fileName);
			
			printf("Successfully read image file '%s'\n", fileName);
			
			
			sobelout= setImage();
			
			temppicx= setImage();
			temppicy= setImage();
			
			sobelfunc(img,sobelout,temppicx,temppicy); 
			
			
			
			printf("Enter image filename for output: ");
			scanf("%s", fileName);
			
			writeoutpic(fileName,sobelout);
			
		}
		
		printf("1.Change values of a picture\n2.Subtract pictures\n3.Sobel edge highlighter\n4.quit\n");
		scanf("%d", &choice);
	}
	
	return(EXIT_SUCCESS);
}


void sobelfunc(int** pic, int** edges, int** tempx, int** tempy){
	
	int maskx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
	int masky[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
	int maxival;
	
	
	
	
	int i,j,p,q,mr,sum1,sum2;
	double threshold;
	
	
	mr = 1;
	
	
	for (i=mr;i<numRows-mr;i++)
	{ for (j=mr;j<numCols-mr;j++)
	{
		sum1 = 0;
		sum2 = 0;
		for (p=-mr;p<=mr;p++)
		{
			for (q=-mr;q<=mr;q++)
			{
				sum1 += pic[i+p][j+q] * maskx[p+mr][q+mr];
				sum2 += pic[i+p][j+q] * masky[p+mr][q+mr];
			}
		}
		tempx[i][j] = sum1;
		tempy[i][j] = sum2;
	}
	}
	
	maxival = 0;
	for (i=mr;i<numRows-mr;i++)
	{ for (j=mr;j<numCols-mr;j++)
	{
		edges[i][j]=(int) (sqrt((double)((tempx[i][j]*tempx[i][j]) +
										 (tempy[i][j]*tempy[i][j]))));
		if (edges[i][j] > maxival)
			maxival = edges[i][j];
		
	}
	}
	
	
	
	for (i=0;i<numRows;i++)
	{ for (j=0;j<numCols;j++)
	{
		edges[i][j] = ((edges[i][j]*1.0) / maxival) * 255;            
		
	}
	}
}

void subtract(int** pic, int** pic2, int** pic3)
{
	int i,j;
	
	for(i=0; i<numRows; i++){
		for (j=0; j<numCols; j++) {
			pic3[i][j]=abs(pic[i][j]-pic2[i][j]);
		}
	}
}

void addtopixels(int** imgtemp, int value)
{  
	int i,j;
	
	for (i=0;i<numRows;i++)
	{ for (j=0;j<numCols;j++)
	{
		imgtemp[i][j] += value;
	}
	}
}

void writeoutpic(char* fileName, int** imgtemp)
{
	int i,j;
	char ci;
	FILE* fo1;
	
	if((fo1 = fopen(fileName, "wb")) == NULL)
	{
		printf("Unable to open out image file '%s'\n", fileName);
		exit(EXIT_FAILURE);
	}
	
	fprintf(fo1,"P5\n");
	fprintf(fo1,"%d %d\n", numRows, numCols);
	fprintf(fo1,"255\n");
	
	for (i=0;i<numRows;i++)
	{ for (j=0;j<numCols;j++)
	{
		ci   =  (char) (imgtemp[i][j]);
		fprintf(fo1,"%c", ci);
	}
	}
}




int** readpic(char* fileName)
{
	FILE* imgFin;
	int** imgtemp;
	
	if((imgFin = fopen(fileName, "rb")) == NULL)
	{
		printf("Unable to open image file '%s'\n", fileName);
		exit(EXIT_FAILURE);
	}
	
	readHeader(imgFin);
	
	
	imgtemp  = setImage();
	
	readBinaryData(imgFin, imgtemp);
	
	fclose(imgFin);
	
	return  imgtemp;
	
}

void readHeader(FILE* imgFin)
{
	int  haveReadImgID   = FALSE;
	int  haveReadImgSize = FALSE;
	int  haveReadMaxVal  = FALSE;
	char line[BUFFER_SIZE];
	
	while(!(haveReadImgID && haveReadImgSize && haveReadMaxVal))
	{
		fgets(line, BUFFER_SIZE, imgFin);
		
		if((strlen(line) == 0) || (strlen(line) == 1))
			continue;
		
		if(isComment(line))
			continue;
		
		if(!(haveReadImgID))
		{
			readImgID(line);
			haveReadImgID = TRUE;
		}
		else if(!(haveReadImgSize))
		{
			readImgSize(line);
			haveReadImgSize = TRUE;
		}
		else if(!(haveReadMaxVal))
		{
			readMaxVal(line);
			haveReadMaxVal = TRUE;
		}
	}
	
}

int isComment(char *line)
{
	if(line[0] == '#')
		return(TRUE);
	
	return(FALSE);
}

void readImgID(char* line)
{
	if(strcmp(line, "P5\n") != 0)
	{
		printf("Invalid image ID\n");
		exit(EXIT_FAILURE);
	}
}

void readImgSize(char* line)
{
	unsigned i;
	
	for(i = 0; i < strlen(line); ++i)
	{
		if(!((isdigit((int) line[i])) || (isspace((int) line[i]))))
		{
			printf("Invalid image size\n");
			exit(EXIT_FAILURE);
		}
	}
	
	sscanf(line, "%d %d", &numRows, &numCols);
}

void readMaxVal(char* line)
{
	unsigned i;
	
	for(i = 0; i < strlen(line); ++i)
	{
		if(!((isdigit((int) line[i])) || (isspace((int) line[i]))))
		{
			printf("Invalid image max value\n");
			exit(EXIT_FAILURE);
		}
	}
	
	maxVal = atoi(line);
}

int** setImage()
{
	int** imgtemp;
	unsigned i;
	
	imgtemp = (int**) calloc(numRows, sizeof(int*));
	
	for(i = 0; i < numRows; ++i)
	{
		imgtemp[i] = (int*) calloc(numCols, sizeof(int));
	}
	return imgtemp;
}

void readBinaryData(FILE* imgFin, int** imgtemp)
{
	unsigned  i;
	unsigned  j;
	for(i = 0; i < numRows; ++i)
	{
		for(j = 0; j < numCols; ++j)
		{
			imgtemp[i][j] = 
			fgetc(imgFin);
		}
	}
}
	
	
