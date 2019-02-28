/**
 * Title:        SKShapeProp
 * Description:  Class that represents an individual shape properties
 */
   package sketcher;

   import java.awt.Color;
   import javax.swing.JComboBox;

   public class SKProp
   {
   //Data
      public String   PropName;
      public Object   PropData = null;
      public int      PropType; //0=individual shape only; 1=shape class only; 2=all
   
      public String[] cmbData;
      public int      editorCode; //Int representing editor, 0=normal textpane
   
   //Functionality
      public boolean isEditable = true;
      public Color   foregroundColor  = Color.black;
      public Color   backgroundColor  = Color.lightGray;
   
      public SKProp(String name, Object data, int type)
      {
         PropName = name;
         PropData = data;
         PropType = type;
      }
   
      public void setComboBoxData(JComboBox cmb)
      {
      //Populate cmb
         cmb.removeAllItems();
         for(int i=0; i<cmbData.length; i++)
         {
            cmb.addItem(cmbData[i]);
         }
      
      //Set selected item to current value
         cmb.setSelectedItem(PropData);
      }
   
      public String toString()
      {
         return PropData.toString();
      }
   }
