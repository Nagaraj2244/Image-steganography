#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */
/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest,EncodeInfo *encInfo)
{
  //int x=encInfo->image_capacity-encInfo->total_file_size;
  /*char *data;
  fread(data, 1, x, fptr_src);
   fwrite(data, 1, x, fptr_dest);
   return e_success;*/
    char ch;
    while(fread(&ch,1,1,fptr_src)>0)
    {
        fwrite(&ch,1,1,fptr_dest);
    }
    return e_success;
   
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
  char data[8];
  char ch;
  //encInfo->size_secret_file;
  for(int i=0;i<encInfo->size_secret_file;i++)
  {
  fread(data,1,8,encInfo->fptr_src_image);
   ch=fgetc(encInfo->fptr_secret);
   encode_byte_to_lsb(ch,data);
   fwrite(data,1,8,encInfo->fptr_stego_image);
  }
  return e_success;
}
Status encode_byte_to_lsb_int(long data, char *image_buffer)
{
   for(int i=31;i>=0;i--)
   {
    image_buffer[31-i]=((image_buffer[31-i])&(~1))|((data>>i)&1);
   }
}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
  char data[32];
  int r_check,w_check;
   r_check=fread(data,1,32,encInfo->fptr_src_image);
  encode_byte_to_lsb_int(file_size, data);
  w_check=fwrite(data,1,32,encInfo->fptr_stego_image);
  if(r_check==32&&w_check==32)
    return e_success;
    else
    return e_failure;
}
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
char data[8];
while(*file_extn!='\0')
{
  fread(data,1,8,encInfo->fptr_src_image);
  encode_byte_to_lsb(*file_extn,data);
  fwrite(data,1,8,encInfo->fptr_stego_image);
  file_extn++;
}
return e_success;
}

Status encode_secret_file_extn_size(long file_size, EncodeInfo *encInfo)
{
  char data[32];
  int w_check,r_check;
    r_check=fread(data,1,32,encInfo->fptr_src_image);
    encode_byte_to_lsb_int(file_size,data);
    w_check=fwrite(data,1,32,encInfo->fptr_stego_image);
    if(r_check==32&&w_check==32)
    return e_success;
    else
    return e_failure;
}
Status encode_byte_to_lsb(char data, char *image_buffer)
{
for(int i=7;i>=0;i--)
image_buffer[7-i]=((image_buffer[7-i])&(~1))|((data>>i)&1);
}
Status encode_data_to_image(const char *data,int size,FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char store[8];
    for(int i=0;i<size;i++)
    {
      fread(store,1,8,fptr_src_image);
      encode_byte_to_lsb(data[i],store);
      fwrite(store,1,8,fptr_stego_image);
    }
    printf("Magic string is successfully fetched\n");
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
  int length=strlen(magic_string);
   encode_data_to_image(magic_string,length,encInfo->fptr_src_image,encInfo->fptr_stego_image);
   return e_success;
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
  char header[54];
  int w_check,r_check;
   fseek(fptr_src_image,0,SEEK_SET);
   r_check=fread(header, 1, 54, fptr_src_image);
   w_check=fwrite(header, 1, 54, fptr_dest_image);
   //w_check=fwrite(header, 1, 54, fptr_stego_image);
   if(r_check==54&&w_check==54)
    {
   
   return e_success;
    }
    else
    return  e_failure;
}

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}
long get_secret_file_size(FILE *fptr_secret)
{
 fseek(fptr_secret,0,SEEK_END);
 long size=ftell(fptr_secret);
 fseek(fptr_secret,0,SEEK_SET);
 
 return size;
}
uint get_file_size(FILE *fptr,EncodeInfo *encInfo)
{
   fseek(fptr,0,SEEK_END); // fseek(fptr,EOF,SEEK_SET)
   uint result=ftell(fptr); //size of secret file
   fseek(fptr,0,SEEK_SET);
   //printf("%u\n",result);
   return 54+(result*8)+(strlen(MAGIC_STRING)*8)+ (8*sizeof(int)) +(8*sizeof(int)) + (strlen((strrchr(encInfo->secret_fname,'.'))) * 8);
}       
long get_extn_size(EncodeInfo *encInfo) 
{
 long ext_len=(strlen(strrchr(encInfo->secret_fname,'.')));
 return ext_len;
}                                       
/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status check_capacity(EncodeInfo *encInfo)
{
encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
printf("%d\n",encInfo->image_capacity);
encInfo->size_secret_file=get_secret_file_size(encInfo->fptr_secret);
 encInfo->size_secret_file_extn_size=get_extn_size(encInfo);
encInfo->total_file_size=get_file_size(encInfo->fptr_secret,encInfo);
//printf("The total size of file we want = %lu\n",encInfo->total_file_size);
if(encInfo->image_capacity>encInfo->total_file_size)
return e_success;
else
return e_failure;
}
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
   if(strstr(argv[2],".bmp")!=NULL)
   {
    if((strcmp(strstr(argv[2],"."),".bmp")==0))
     encInfo->src_image_fname=argv[2];
   }
     else
    return e_failure;

    if((strstr(argv[3],".txt")!=NULL)||(strstr(argv[3],".c")!=NULL)||(strstr(argv[3],".sh")!=NULL))
    {
    if((strcmp(strrchr(argv[3],'.'),".txt")==0||strcmp(strrchr(argv[3],'.'),".c")==0||strcmp(strrchr(argv[3],'.'),".sh")==0))
    {
      encInfo->secret_fname=argv[3];
    }
    }
    else
    return e_failure;

   if(argv[4]==NULL)
    encInfo->stego_image_fname="stego.bmp";
      else
     {
     if((strstr(argv[4],".bmp")!=NULL))
     {
     if((strcmp(strstr(argv[4],"."),".bmp")==0))
     {
        encInfo->stego_image_fname=argv[4];
     }
     }
     else
        return e_failure;
     }

     return e_success;
}
Status do_encoding(EncodeInfo *encInfo)
{
if(open_files(encInfo)==e_success)
  {
    printf("successfully open the file\n");
  }
else
    return e_failure;
if(check_capacity(encInfo)==e_success)
  {
    printf("You can store secret data inside sorce file\n");
  }
else
    return e_failure;

if((copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image))==e_success)
   {
    printf("The header data is raed and fetched successfully\n");
   }
else
    return e_failure;
if(encode_magic_string(MAGIC_STRING, encInfo)==e_success)
   {
    printf("Encoding magic string is completed\n");
   }
else
    return e_failure;
if((encode_secret_file_extn_size(encInfo->size_secret_file_extn_size,encInfo))==e_success)
    {
      printf("The secret file extension size is fetched to stego file\n");
    }
else
    return e_failure;
if(encode_secret_file_extn(strrchr(encInfo->secret_fname,'.'), encInfo)==e_success)
   {
    printf("The secret file extension is fetched to stego file\n");
   }
else
   return e_failure;
if((encode_secret_file_size(encInfo->size_secret_file,encInfo))==e_success)
    {
      printf("The secret file size is fetched to stego file\n");
    }
else
    return e_failure;
if(encode_secret_file_data(encInfo)==e_success)
   {
     printf("The secret file data is fetched to stego file\n");
   }
else
   return e_failure;
if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image,encInfo)==e_success)
  {
    printf("All the data fetched succussfully\n");
  }
else
    return e_failure;

return e_success;
}