#include <setjmp.h>

#include "tlpi_hdr.h"

/* (Refer K&R Section 4.6: Static variables)
 *
 * The static declaration, applied to an external variable or function,
 * limits the scope of that object to the rest of the source file 
 * being compiled. External static thus provides a way to hide names 
 * like buf and bufp in the getch-ungetch combination, which must be 
 * external so they can be shared, yet which should not be visible 
 * to users of getch and ungetch .
 */
static jmp_buf env;

static int
setJumpLoc()
{
  int val;
  val = setjmp(env);
  printf("This is site after initial return from setjmp()"
	  " inside setJumpLoc() function\n");
  return val;
}

static void
doLongJump()
{
  longjmp(env, 1);
}


int
main()
{
  int val;

  /* Set jump location by calling a function */
  switch(setJumpLoc()) {
	case 0:
	  printf("This is site after initial return from setjmp()"
		  " inside caller of setJumpLoc()\n");
	  doLongJump(); /* Never returns... */
	  break;
	case 1:
	  printf("This is the site after return from"
	  " doLongJump(), inside its caller function\n");
	  break;
  }

  exit(EXIT_SUCCESS);
}
