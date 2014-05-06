
/**
 * Title:        JStatusBarPanel
 * Description:  Represents 1 panel of a JStatusBar
 */
   package sketcher;

   public class JStatusBarPanel
   {
      public String text;
      public int    size;
      public int    align; //0=left, 1=centerl, 2=right
   
      public JStatusBarPanel(String txt, int sz, int algn)
      {
         text = txt;
         size = sz;
         align = algn;
      }
   }