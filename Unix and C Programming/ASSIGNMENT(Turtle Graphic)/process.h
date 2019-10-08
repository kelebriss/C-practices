#ifndef __process_h__
#define __process_h__

void process(dataList* list, outputList* output); /*
						  **doing the draw and move also set current position, current angle,
						  ** current FG and FB also current pattern
						  */

void plotting (void *data); /*this function is printing the data's patttern is can be assigned by PlotFunc*/
double round(double a); /*round function for taking nearest value for drawing coodinate*/

#endif

