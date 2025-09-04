#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

typedef struct _DecodeInfo
{
    /* Source Image info "beautiful.bmp"*/
   /* char *src_image_fname; //To store the source file name
    FILE *fptr_src_image; //To store the source file
    uint image_capacity;
    uint bits_per_pixel;
    //char image_data[MAX_IMAGE_BUF_SIZE];
    long total_file_size;*/

    /* Secret File Info */
   /* char *secret_fname;  //To store secret file name
    FILE *fptr_secret;   //To store the secret file
   // char extn_secret_file[MAX_FILE_SUFFIX];
    //char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;*/

    /* Stego Image Info */
    char *stego_image_fname;  //To store output file name
    FILE *fptr_stego_image;  //To store yhe output file

    char output_fname[100];
    //char file_name[1000];
    FILE *fptr_output_file;

     char *decoded_mgc_str;
     char *decoded_secret_file_ext;
     char *decoded_secret_file;

   int decoded_extn_size;
   int decoded_secret_file__size;

} DecodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_files_decode(DecodeInfo *decInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
//uint get_file_size(FILE *fptr,DecodeInfo *decInfo);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);

Status decode_secret_file_extn_size(DecodeInfo *decInfo);
/* Encode secret file extenstion */
//Status decode_secret_file_extn(const char *file_extn, DecodeInfo *decInfo);
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Encode secret file size */
Status decode_secret_file_size(long file_size, DecodeInfo *decInfo);

/* Encode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Encode function, which does the real encoding */
Status decode_image_to_data(int size,DecodeInfo *decInfo);

/* Encode a byte into LSB of image data array */
Status decode_byte_to_lsb(int size, DecodeInfo *decInfo, char *image_buffer,char *mg_str);

#endif
