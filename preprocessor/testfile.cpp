#define PICARD "AU_CONTRAIRE_MON_CAPITAINE"
char abuf[] = PICARD;
#undef PICARD
#define HASHSIZE 1000
int j = HASHSIZE, k = 2 * HASHSIZE;
#undef HASHSIZE
#define PICARD "only reason I come is to listen to these wonderful speeches of yours"
char anotherbbuf[] = PICARD;
// this is a comment with PICARD in it -- it shouldn't have its defn replace it
#undef PICARD
/* this is a comment */
// this is a single line comment
#define HASHSIZE 1000
int i = HASHSIZE;
const char* ps = "all they left me is my bones";
//this is a comment with HASHSIZE in it -- it should not be replaced by its defn! */
#undef HASHSIZE
/* END OF FILE */