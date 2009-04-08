/*
    Copyright (C) 2008, 2009 Svante Hermansson

    Please note that this ugly hack makes the ASF-file unplayable
    in MS windows media player... but who cares? :p
    For info or contact: http://www.dillchip.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	FILE *fin, *fout;
  size_t mysize;
	int val, i;

// GUID's:  (asfheader.h)
//    { "audio media",
//    { 0xf8699e40, 0x5b4d, 0x11cf, { 0xa8, 0xfd, 0x00, 0x80, 0x5f, 0x5c, 0x44, 0x2b }} },

  int audio[8]={0x40, 0x9E, 0x69, 0xF8, 0x4D, 0x5B, 0xCF, 0x11};  //only checks the first 8 bytes... :o
  int audioi=0;
	int audiofound=0;
  int offset=0;
  int lastaudio=0;


  if (argc==2){
	  printf("[inputfile=%s]\n", argv[1]);
	  //printf("[outputfile=%s]\n", argv[2]);
	} else {
		printf("more arguments needed... %s <file_to_patch>\n", argv[0]);
		exit(1);
  }

  fin=fopen(argv[1], "rb");

	for (i=0;i<20;i)
  {
		val = fgetc(fin);
		if (val==EOF)
			break;

		//check for audio stream
    if (val==audio[audioi])
		{
			if (audioi==7)
			{
				printf("found audio stream at offset 0x%x\n", offset-7);
				audioi=0;
			  lastaudio = offset - 7;  // mark this audio block
				audiofound++;
			}
			else
				audioi++;  // this is correct, but we need to find the full string.
		}
		else   // this is not it. keep looking.
			audioi=0;

		offset++;
		if (offset == 0xA000) // all headers should be in this memory, right?
			break;		
	}
	//printf("file size 0x%x, %d\n", offset, offset);
  fclose(fin);

  // TRY TO PATCH THE FILE
	if (lastaudio!=0)
	{
		if (audiofound!=1)
		{
			printf("Will now patch at offset 0x%x.", lastaudio);  
		  fout=fopen(argv[1], "r+b");
			fseek (fout, lastaudio+1, SEEK_SET); //go to patch place;
			fputc(0, fout);
#if 0
				val = fgetc(fin);
				printf("[%x]=>",val);
				val = fgetc(fin);
				printf("[%x]=>",val);
				val = fgetc(fin);
				printf("[%x]=>",val);
				val = fgetc(fin);
				printf("[%x]=>\n",val);
#endif			
			fclose(fout);			
			printf(" DONE! OK! \n");
		}
		else
			printf("Only one audio stream found, no need to patch!\n");
	
	}
	else
		printf("found no audio, no patching done!\n");	
}

