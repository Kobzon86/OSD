#ifndef FONTS_H_
#define FONTS_H_

/***********************************************************
 *                           TYPES                         *
 ***********************************************************/

typedef struct{
  unsigned long extents_width;
  unsigned long extents_height;
  unsigned long extents_ascent;
  unsigned long extents_descent;
  unsigned long bounds_width;
  unsigned long bounds_height;
  unsigned char *char_alpha_map;
  unsigned long reserved;
} font_struct;

typedef struct {
  font_struct *font;
  char* chars[94];
} prerendered_font;

/***********************************************************
 *                      EXTERN VARIABLES                   *
 ***********************************************************/

extern font_struct arialbold_15[];
extern font_struct arialbold_34[];
extern font_struct arialbold_60[];
extern font_struct digital7_30[];
extern font_struct segoescriptbold_42[];

#endif /* FONTS_H_ */
