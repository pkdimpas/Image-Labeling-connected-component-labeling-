/**
 * File        : label-philo.c
 * Usage       : label-philo (filename.pbm)
 * Description : This program will accept a PBM (P4 format only)
 *               image and will perform connected component
 *               labeling
 * Date:       : Dec 29, 2018
 * Author      : Philogene Kyle Dimpas
 */

#include <stdio.h>
#include <stdlib.h>

#define EXT_SUCCESS  1
#define EXT_FAILURE  0
#define PIXEL        255
#define BACKGROUND   0

/** readability purpose */
#define matrix(i, j)    *(img.matrix + j + i * img.width)
#define DISPLAY_ERROR(...){ printf("ERROR: "__VA_ARGS__);   \
                            return EXT_FAILURE;             \
}

typedef unsigned char uchar;
/**
 * Description:
 *      holds the data of PBM P4 image and labeled
 *      image
 * members:
 *      - height  height of the image
 *      - width   width of the image
 *      - pbm     holds the original P4 image
 *      - matrix  holds the labeled image
 */
typedef struct{
    int     height;
    int     width;
    uchar   *pbm;
    uchar   *matrix;
}PBMImage;

/**
 * Description:
 *      gets the width and height from P4 header
 * Parameters :
 *      - fp_pbm    points to the opened P4 image
 *      - img       PBMImage structure
 * Returns:
 *      - EXT_FAILURE  failed in parsing. not P4
 *      - EXT_SUCCESS  success in parsing
 */
int processHeader(FILE *fp_pbm, PBMImage *img)
{
    int     i = 0;
    int     cnt = 0;
    int     pbm_header_info[2];
    char    buff[3];
    char    comment[256];
    char    ch;

    if (fgetc(fp_pbm) != 'P' || fgetc(fp_pbm) != '4')
        return EXT_FAILURE;

    while (cnt < 2){
        ch = fgetc(fp_pbm);
        /* check the #comment strings */
        if (ch != '#'){
            /* find the first character of the data */
            if (!isspace(ch)){
                buff[i++] = ch;
            } else if (i > 0){
            /* stores parsed data */
                buff[i] = '\0';
                pbm_header_info[cnt++] = atoi(buff);
                i = 0;
            }
        } else {
            /* read to the end of the comment */
            fgets(comment, sizeof(comment), fp_pbm);
        }
    }

    img->width  = pbm_header_info[0];
    img->height = pbm_header_info[1];

    return EXT_SUCCESS;
}

/**
 * Description:
 *      displays the labeled image using the matrix
 * Parameters :
 *      - img   PBMImage structure
 */
void displayLabeledImage(PBMImage img)
{
    int i;
    int j;

    for (i = 0; i < img.height; i++){
        for (j = 0; j < img.width; j++){
            if (matrix(i, j) != BACKGROUND)
                printf("%2d", matrix(i,j));
            else
                printf("%2c", BACKGROUND);
        }
        printf("\n");
    }
}

/**
 * Description:
 *      performs image labeling using DFS graph traversal(recursion)
 * Parameters :
 *      - img PBMImage structure
 * Returns:
 *      - total number of colors used
 */
int performImageLabeling(PBMImage img)
{
    int     i;
    int     j;
    int     label = 1;
    uchar  *tmp = img.pbm;
    uchar   bit;

    /* initialization of matrix */
    for (i = 0; i < img.height; i++){
        for (bit = 7, j = 0; j < img.width; j++, bit--){
            matrix(i, j) = *tmp & (0x01 << bit) ? PIXEL : BACKGROUND;
            if (bit == 0){
                tmp++;
                bit = 8;
            }
        }
        /* tmp is already incremented if divisble by 8 */
        if (img.width % 8 != 0)
            tmp++;
    }

    /* labeling */
    for (i = 0; i < img.height; i++){
        for (j = 0; j < img.width; j++){
            if (matrix(i, j) == PIXEL)
                depthFirstSearch(img, i , j, label++);
        }
    }

    return label - 1;
}

 /**
 * Description:
 *      a recursive algorithm that involves exhaustive searches of all
 *      the nodes by going ahead, if possible, else by backtracking.
 *      marking visited nodes as labeled.
 *
 *                1(7, 13)
 *              /   \
 *           2(4, 6) 8(10, 12)      tree representation
 *            / \   / \             (n) - backtrack
 *           3  5  9   11
 *
 * Parameters :
 *      - img       PBMImage structure
 *      - column    current column index
 *      - row       current row index
 *      - label     current label color
 * Returns:
 *      - 0         out of bounds / deepest node
 */
int depthFirstSearch(PBMImage img, int column, int row, int label)
{
    int r;
    int c;

    /* check bounderies */
    if (row < 0 || column < 0 || row >= img.width || column >= img.height)
        return 0;

    if (matrix(column, row) == BACKGROUND || matrix(column, row) == label)
        return 0;

    matrix(column, row) = label;
    /**
     * check neighbors
     * column == height
     * row == width
     */
    for (c = column - 1 ; c <= column + 1; c++){
        for (r = row - 1; r <= row + 1; r++){
            /* self check */
            if (r != row || c != column)
                depthFirstSearch(img, c, r, label);
        }
    }

    return 0;
}

/**
 * Description:
 *      main function
 * Parameters:
 *      - argc  number of arguments
 *      - argv  array of arguments
 * returns:
 *      - EXT_SUCCESS operation success
 *      - EXT_FAILURE operation failed
 */
int main(int argc, char *argv[])
{
    int          datasize = 0;
    FILE        *fp_pbm = NULL;
    PBMImage     img;


    if (argc < 2)
        DISPLAY_ERROR("USAGE: label-philo (filename.pbm)");

    fp_pbm = fopen(argv[1], "rb");

    if (!fp_pbm)
        DISPLAY_ERROR("File cannot be opened!");

    if (!processHeader(fp_pbm, &img))
        DISPLAY_ERROR("File is not in P4 format");

    /* allocate memory for the original PBM*/
    datasize = (img.width / 8) + (img.width % 8 == 0 ? 0: 1);
    img.pbm = (uchar*)malloc(sizeof(char) * datasize * img.height);

    if (!img.pbm)
        DISPLAY_ERROR("'img.pbm' failed to allocate a memory");

    if (fread(img.pbm, 1, sizeof(char) * datasize * img.height, fp_pbm) < datasize){
        free(img.pbm);
        DISPLAY_ERROR("Data read is less than the expected data size");
    }

    /* allocate memory for matrix */
    img.matrix = (uchar*)malloc(sizeof(char) * img.height * img.width);

    if (!img.matrix){
        free(img.pbm);
        DISPLAY_ERROR("'img.matrix' failed to allocate a memory\n");
    }

    printf("Input file: %d (W) X %d (H)\n", img.width, img.height);
    printf("Color used: %d\n", performImageLabeling(img));
    displayLabeledImage(img);

    free(img.pbm);
    free(img.matrix);
    return EXT_SUCCESS;
}
