#include <stdio.h>
#include<stdlib.h>

#define and &&
#define or ||

int esc_seq(char c){
    char seq[] = "abfnrtv0'\\\?";
    int i = 0;
    while(seq[i] != c) i++;
    return seq[i] == c;
}

struct Tree
{
    char character;
    int count;
    struct Tree* parent;
    struct Tree* left;
    struct Tree* right;
};

struct Tree* search(struct Tree* root, char c)
{
    if (root->character == c)
        return root;
    else
    {
        if (root->character < c)
        {
            if (root->right == NULL)
                return root;
            return search(root->right, c);
        }
        else if (root->character > c)
        {
            if (root->left == NULL)
                return root;
            return search(root->left, c);
        }
    }
}

void Zig_Left(struct Tree* root)
{
    //printf("Zig Left\n");
    root->parent->left = root->right;
    root->right = root->parent;
    root->parent = root->right->parent;
    root->right->parent = root;
    if (root->right->left != NULL)
        root->right->left->parent = root->right;
    if (root->parent != NULL)
    {
        if (root->parent->left == root->right)
            root->parent->left = root;
        else if (root->parent->right == root->right)
            root->parent->right = root;
    }
}

void Zig_Right(struct Tree* root)
{
    //printf("Zig Right\n");
    root->parent->right = root->left;
    root->left = root->parent;
    root->parent = root->left->parent;
    root->left->parent = root;
    if (root->left->right != NULL)
        root->left->right->parent = root->left;
    if (root->parent != NULL)
    {
        if (root->parent->left == root->left)
            root->parent->left = root;
        else if (root->parent->right == root->left)
            root->parent->right = root;
    }
}

void Zig_Zig_Left(struct Tree* root)
{
    //printf("Zig Zig Left\n");
    Zig_Left(root->parent);
    Zig_Left(root);
}

void Zig_Zig_Right(struct Tree* root)
{
    //printf("Zig Zig Right\n");
    Zig_Right(root->parent);
    Zig_Right(root);
}

void Zig_Zag_Left(struct Tree* root)
{
    //printf("Zig Zag Left\n");
    Zig_Right(root);
    Zig_Left(root);
}

void Zig_Zag_Right(struct Tree* root)
{
    //printf("Zig Zag Right\n");
    Zig_Left(root);
    Zig_Right(root);
}

struct Tree* new_tree(struct Tree* root, char c)
{
    struct Tree* place = (struct Tree*)malloc(sizeof(struct Tree));
    if (root == NULL)
    {
        struct Tree* new_root = (struct Tree*)malloc(sizeof(struct Tree));
        new_root->character = c;
        new_root->count = 1;
        new_root->parent = NULL;
        new_root->left = NULL;
        new_root->right = NULL;
        place = new_root;
    }
    else
    {
        place = search(root, c);
        //printf("search place %d\n", place->character);
        if (place->character != c)
        {
            struct  Tree* new_root = (struct Tree*)malloc(sizeof(struct Tree));
            new_root->character = c;
            new_root->count = 1;
            new_root->parent = place;
            new_root->left = NULL;
            new_root->right = NULL;
            if (place->character < c)
                place->right = new_root;
            else
                place->left = new_root;
            place = new_root;
        }
        else
            place->count++;
    }
    while (place->parent != NULL)
    {
        if (place->parent->parent != NULL)
        {
            //printf("parent %d\nparent parent %d\n",place->parent->character, place->parent->parent->character);
            if (place == place->parent->right && place->parent == place->parent->parent->left)
                Zig_Zag_Left(place);
            else
            if (place == place->parent->left && place->parent == place->parent->parent->right)
                Zig_Zag_Right(place);
            else
            if (place == place->parent->left && place->parent == place->parent->parent->left)
                Zig_Zig_Left(place);
            else
                Zig_Zig_Right(place);
        }
        else
        {
            if (place == place->parent->left)
                Zig_Left(place);
            else
                Zig_Right(place);
        }
    }
    return place;
}

void print_tree(struct Tree *x) {
    if (x == NULL) return;
    printf("Char %c встречается %d раз%c\n", x->character, x->count, (6 > x->count and x->count > 1) ? 'a': ' ');
//    printf( (6 > x->count and x->count > 1) ?  : "Char %c встречается %d раз\n", x->character, x->count);
    print_tree(x->left);
    print_tree(x->right);
}

int main() {
    //очистка комментариев
    FILE *input, *output;
    char sym, tmp_sym;
    int counter = 0;
    input = fopen("../test.cpp", "rt");
    output = fopen("../test.txt", "wt");
    while (!feof(input)){
        sym = fgetc(input);
        counter = 0;
        if (sym == '"'){
            do{
                fputc(sym, output);
                tmp_sym = sym;
                if (sym == '\\') counter++;
                else { counter = 0; }
                sym = fgetc(input);
            } while(!feof(input) and (sym != '\n' or counter % 2 or tmp_sym == 13) and (sym != '"' or counter % 2));
        }
        else if (sym == '\''){
            do{
                fputc(sym, output);
                if (sym == 92) counter++;
                else { counter = 0; }
                sym = fgetc(input);
            } while(!feof(input) and (sym != '\n' or counter % 2 or tmp_sym == 13) and (sym != '\'' or counter % 2));
        }
        else{
            if (sym == '/'){
                sym = fgetc(input);
                if (sym == '/'){
                    do{
                        if (sym == '\\') counter++;
                        else{ counter = 0; }
                        sym = fgetc(input);
                    }while(!(sym == '\n' and !counter) and !feof(input));
                    fputc('\n', output);
                    continue;
                }
                else if(sym == '*'){
                    tmp_sym = fgetc(input);
                    do{
                        sym = tmp_sym;
                        tmp_sym = fgetc(input);
                    }while((tmp_sym != '/' or sym != '*') and !feof(input));
                    continue;
                }
                else if(sym == '"'){
                    fputc('/', output);
                    do{
                        fputc(sym, output);
                        tmp_sym = sym;
                        if (sym == '\\') counter++;
                        else{ counter = 0; }
                        sym = fgetc(input);
                    } while(!feof(input) and (sym != '\n' or counter % 2 or tmp_sym == 13) and (sym != '"' or counter % 2));
                }
                else if(sym == '\''){
                    fputc('/', output);
                    do{
                        fputc(sym, output);
                        tmp_sym = sym;
                        if (sym == '\\') counter++;
                        else { counter = 0; }
                        sym = fgetc(input);
                    } while(!feof(input) and (sym != '\n' or counter % 2 or tmp_sym == 13) and (sym != '\'' or counter % 2));
                }
                else {
                    fputc('/', output);
                }
            }
        }
        if(!feof(input)) fputc(sym, output);
    }
    fclose(input);
    fclose(output);

    struct Tree *main_tree = (struct Tree*) malloc(sizeof(struct Tree*));
    main_tree = NULL;
    FILE *f = fopen("../test.txt", "rt");
    char c;
    char f1;
    while ((c = fgetc(f)) != EOF) {
        f1 = 1;
        if (c == '\'') {
            char c2, c3;
            c2 = fgetc(f);
            if (c2 == '\\'){
                c2 = fgetc(f);
                f1 = 0;
            }
            if (!f1 && esc_seq(c2)){
                switch (c2) {
                    case 'n': {
                        c2 = '\n';
                        break;
                    }
                    case 't':{
                        c2 = '\t';
                        break;
                    }
                    case 'a': {
                        c2 = '\a';
                        break;
                    }
                    case 'b':{
                        c2 = '\b';
                        break;
                    }
                    case 'f':{
                        c2 = '\f';
                        break;
                    }
                    case 'r':{
                        c2 = '\r';
                        break;
                    }
                    case 'v':{
                        c2 = '\v';
                        break;
                    }
                    case '?':{
                        c2 = '\?';
                        break;
                    }
//                    default:{
//                        printf("Не туда поворот -----%c-----\n", c2);
//                        break;
//                    }
                }
            }
            c3 = fgetc(f);
            if (c3 == '\'' and (esc_seq(c2) or f1)) {
                main_tree = new_tree(main_tree, c2);
            }
        }
    }
    print_tree(main_tree);
    fclose(f);
    return 0;
}
