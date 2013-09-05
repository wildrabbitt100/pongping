









#include <stdio.h>
#include <math.h>
#include <stdlib.h>






int main()
{
   double tan0;
   double tan45, tan90;

   tan0 = atan2( 0, 10 );

   
   
   printf(" atan2( 0, 10 )   (tan of 0) is %f\n\n", atan2( 0, 10 ) );

   printf(" atan2( 0, - 10 ) (tan of 180) is %f\n\n", atan2( 0, - 10 ) );
   
   tan90 = atan2( 10, 0 );
   
   printf(" atan2( 10, 0 ) tan of 90 is %f\n\n", tan90 );

   printf(" atan2( - 10, 0 ) tan of 270 is %f\n\n", atan2( -10, 0) );
   
   tan45 = atan2(10, 10);
   
   printf(" tan of 45 is %f\n\n", tan45 );
   
   
   printf(" tan(0, 0) = %f\n\n" , atan2(0.0, 0.0));
   
   system("PAUSE");
}