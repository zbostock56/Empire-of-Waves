/*
                                DIALOG_STR.h
Describe the struct representing a dialog. Can be included in any file which 
reads/manipulates a dialog.
*/

#define MAX_NAME (16) // Max string length of name
#define MAX_CONTENT (256) // Max string length of content

typedef struct dialog {
    char *name;
    char *content;
} DIALOG;
