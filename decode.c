#include <stdio.h>
#include<string.h>
#include "decode.h"
#include "types.h"
#include "common.h"
Status decode_secret_file_data(DecodeInfo *decInfo)
{
  char data[decInfo->decoded_secret_file__size*8];
  char secret[decInfo->decoded_secret_file__size*8+1];
  fread(data,1, decInfo->decoded_secret_file__size*8,decInfo->fptr_stego_image);
  decode_byte_to_lsb(decInfo->decoded_secret_file__size, decInfo, data,secret);
  decInfo->decoded_secret_file=secret;
  fprintf(decInfo->fptr_output_file,"%s",decInfo->decoded_secret_file);
  //printf("%s\n",decInfo->decoded_secret_file);
  return e_success;
}
int decode_byte_from_lsb_int(DecodeInfo *decInfo, char *image_buffer)
{
    int data = 0;
    for(int i = 0; i < 32; i++)  
    {
        data = (data << 1) | (image_buffer[i] & 1);  
    }  
    return data;
}
Status decode_secret_file_size(long file_size, DecodeInfo *decInfo)
{
  char data[32];
  fread(data,1,32,decInfo->fptr_stego_image);
   decInfo->decoded_secret_file__size=decode_byte_from_lsb_int(decInfo,data);
   return e_success;
}
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char data[decInfo->decoded_extn_size * 8];
    char file_ext[decInfo->decoded_extn_size+1];
    fread(data,1,decInfo->decoded_extn_size* 8,decInfo->fptr_stego_image);
    decode_byte_to_lsb( decInfo->decoded_extn_size, decInfo, data,file_ext);
    decInfo->decoded_secret_file_ext=file_ext;
   strcpy(decInfo->output_fname,strcat(decInfo->output_fname,decInfo->decoded_secret_file_ext));
   return e_success;
    /*char file[strlen(decInfo->output_fname)+strlen(decInfo->decoded_secret_file_ext)+1];
    int i;
    for(i=0;i<strlen(decInfo->output_fname);i++){
      file[i]=decInfo->output_fname[i];
    }
    int j,k=0;
    char *ptr=strcat(file,decInfo->decoded_secret_file_ext);
    memmove(decInfo->file_name,ptr,sizeof(file));*/
}


Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
  char data[32];
  fread(data,1,32,decInfo->fptr_stego_image);
  decInfo->decoded_extn_size=decode_byte_from_lsb_int(decInfo,data);
  return e_success;
}

Status decode_byte_to_lsb(int size, DecodeInfo *decInfo, char *image_buffer,char *mg_str)
{
    char data = 0;
    for(int j = 0; j < size; j++)  
    {
     data = 0;
        for(int i = 0; i < 8; i++)  
        {
            data = (data << 1) | (image_buffer[i] & 1);  
        }
        image_buffer += 8;  
        mg_str[j] = data;  
    }
    mg_str[size]='\0';
}
Status decode_image_to_data(int size,DecodeInfo *decInfo)
{
   char data[size*8];
   char mg_str[size+1];
  fseek( decInfo->fptr_stego_image,54,SEEK_SET);
  fread(data,1,size*8, decInfo->fptr_stego_image);
 decode_byte_to_lsb(size, decInfo,data,mg_str);
 decInfo->decoded_mgc_str=mg_str;
// if(decInfo->decoded_mgc_str!=MAGIC_STRING)
 //return e_failure;

}
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
  int length=strlen(magic_string);
   decode_image_to_data(length,decInfo);
   if(strcmp(decInfo->decoded_mgc_str,MAGIC_STRING)!=0)
 return e_failure;
 else
   return e_success;
}

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
  if((strstr(argv[2],".bmp")!=NULL))
  {
  if((strcmp(strrchr(argv[2],'.'),".bmp")==0))
  {
    decInfo->stego_image_fname=argv[2];
  }
  }
     else
    return e_failure;

   if(argv[3]==NULL)
   strcpy(decInfo->output_fname,"output");
   //decInfo->output_fname="output";
   else
   {
   strcpy(decInfo->output_fname,strtok(argv[3],"."));
   //decInfo->output_fname=strtok(argv[3],".");
   }

return e_success;
}
Status open_files_decode(DecodeInfo *decInfo)
{
  // Stego Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

    	return e_failure;
    }
    return e_success;
}
Status open_file_for_write(DecodeInfo *decInfo)
{
    decInfo->fptr_output_file=fopen(decInfo->output_fname,"w");
    // No failure return e_success
    return e_success;  
}
Status do_decoding(DecodeInfo *decInfo)
{
  if(open_files_decode(decInfo)==e_success)
  printf("Successfully opened the file\n");
  else
  return e_failure;
   if(decode_magic_string(MAGIC_STRING,decInfo)==e_success)
   printf("Decoded the magic string\n");
   else
   return e_failure;
 if(decode_secret_file_extn_size(decInfo)==e_success)
 printf("Decoded secret file extension size\n");
 else
 return e_failure;
  if(decode_secret_file_extn(decInfo)==e_success)
  printf("Decoded secret file extension\n");
  else
  return e_failure;
  if(open_file_for_write(decInfo)==e_success)
  printf("successfully opened the file for write\n");
  else
  return e_failure;
 if(decode_secret_file_size(sizeof(int),decInfo)==e_success)
  printf("Decoded secret file size\n");
  else
  return e_failure;
  if(decode_secret_file_data(decInfo)==e_success)
  printf("Decoded secret file data\n");
  else
  return e_failure;

  return e_success;
}