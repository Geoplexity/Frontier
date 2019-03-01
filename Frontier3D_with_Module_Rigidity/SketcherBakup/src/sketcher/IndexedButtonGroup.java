/**
 * Title:        IndexedButtonGroup
 * Description:  Works like a button group, except it keeps and int that indicates which button is selected
 */
   package sketcher;

   import javax.swing.ButtonGroup;
   import javax.swing.AbstractButton;
   import javax.swing.ButtonModel;

   public class IndexedButtonGroup extends ButtonGroup
   {
      public IndexedButtonGroup()
      {
         super();
      }
   
      public int getSelectedIndex()
      {
         ButtonModel bm = getSelection();
         if (bm == null)
         {
            return -1;
         }
         else
         {
            for (int g=0; g<buttons.size(); g++)
            {
               if ( ((AbstractButton)buttons.get(g)).getModel().equals(bm) )
                  return g;
            }
         
            return -1;
         }
      }
   }