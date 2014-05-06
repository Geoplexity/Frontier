/**
 * Title:        MRUItem
 * Description:  MRU item
 */
   package sketcher;

   public class MRUItem
   {
      public String displayText;
      public String data;
   
      public MRUItem(String displayTxt, String MRUdata)
      {
         displayText = displayTxt;
         data = MRUdata;
      }
   }