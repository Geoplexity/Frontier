package sketcher;

import java.util.StringTokenizer;
import java.io.*;

/**
<B>stdio</B> defines static methods for using typed input from the console window.  This
class includes two methods for getting a line of input, two methods for
handling (i.e. parsing) typed-in numbers, and a simple rounding method so that
System.out.println won't print "0.33333333333333333" when you want to see "0.33".
<BR><BR>
<I>Known uses:</I> I have used this class successfully as a way to let my students
produce command-line applications that they can run in a MS-DOS command line window.
This lets them get started without first having to climb the learning curve for
Java GUI programming (AWT and Swing).
<BR><BR>
<I>Hints for use:</I> Put stdio.java and stdio.class into your project folder and
then use stdio in your project as you would use any other class.
<BR><BR>
<I>Version 1.0</I>
*/
public class stdio
{
  private static BufferedReader    in = new BufferedReader(new InputStreamReader(System.in));

  /**
   rounds a double to 2 or fewer digits to the right of the decimal point
   */
  public static double round2 (double x)
  {
    return ((double) Math.round(x * 100)) / 100;
  }

  /**
   attempts to parse input string into an Integer.  Returns null if it can't.
   */
  public static Integer parseInteger (String s)
  {
    int     n;

    try {
      return new Integer(s);
      }
    catch (NumberFormatException e) {
      return null;
      }
  }

  /**
   attempts to parse input string into a Double.  Returns null if it can't.
   */
  public static Double parseDouble (String s)
  {
    int     n;

    try {
      return new Double(s);
      }
    catch (NumberFormatException e) {
      return null;
      }
  }

  /**
   returns line of typed input as an array of Strings.  Each String in the array contains
   one word of the input.  If the line is empty, returns null.
   */
  public static String[] getLineTokens ()
  {
    String            line = getLine();
    StringTokenizer   st;
    int               nTokens;
    String[]          tokens = null;

    if (line.length() > 0) {
      // we have a line
      st = new StringTokenizer(line);
      nTokens = st.countTokens();
      if (nTokens > 0) {
        // and that line has at least one word in it (e.g. not just spaces)
        tokens = new String[nTokens];
        for (int i = 0; i < nTokens; i++)
          tokens[i] = st.nextToken();
        }
      }
    return tokens;
  }

  /**
   returns line of typed input as a String.  If the line is empty, returns a zero-length string.
   */
  public static String getLine ()
  {
    try {
      return in.readLine();
      }
    catch (IOException e) {
      return "";
      }
  }

}
