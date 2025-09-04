#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc,char **argv)
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
     uint img_size;
    if(argc==1)
    {
        printf("The minimum number of argument for encoding is 4\nMinimum number of argument for decoding is 3\n");
        return 0;
    }
if(check_operation_type(argv)==e_encode)
{
    if(argc<4)
     {
        printf("The minimum number of argument for encoding is 4\n");
        return 0;
     }
     printf("selected Encoding part\n");
        if(read_and_validate_encode_args(argv,&encInfo)==e_success)
        {
          printf("Read and validate file name successfull\n");
           if(do_encoding (&encInfo)==e_success)
               printf("Encoding successfull\n");
            else
             {
               printf("Encoding failed\n");
                return 0;
             }
       }
       else
       {
          printf("Read and validation failed\n");
           return 0;
       }
}

else if(check_operation_type(argv)==e_decode)
{
    if(argc<3)
     {
        printf("Minimum number of argument for decoding is 3\n");
        return 0;
     }
     printf("You have selected decoding\n");
     if(read_and_validate_decode_args(argv,&decInfo)==e_success)
     {
        printf("Read and validate file name successfully\n");
        if(do_decoding(&decInfo)==e_success)
          printf("Decoding successful\n");
        else
        {
           printf("Decoding_failed\n");
           return 0;
       }
     } 
    else
    {
        printf("Read and validation failed\n");
        return 0;
    }
}
else
{
  printf("Invalid Argument\n");
  return 0;
}   
}

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e")==0)
    return e_encode;
    else if(strcmp(argv[1],"-d")==0)
    return e_decode;
    else
    return e_unsupported;
}
