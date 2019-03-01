/**
 * Title:        MultiCellEditor
 * Description:  Allows for multiple types of cell editors for a single column
 */
   package sketcher;

   import javax.swing.DefaultCellEditor;
   import javax.swing.JComboBox;
   import javax.swing.JTextField;
   import javax.swing.JTable;
   import java.awt.Component;
   import java.awt.event.ItemEvent;

   public class MultiCellEditor extends DefaultCellEditor
   {
      JComboBox comboEditor = new JComboBox();
      int       thisEditor; //0 = super's editor, >0 = custom editor
   
      public MultiCellEditor()
      {
         super(new JTextField());
      
         setClickCountToStart(1);
      
         comboEditor.addItemListener(
                                 new java.awt.event.ItemListener()
                                 {
                                    public void itemStateChanged(ItemEvent e)
                                    {
                                       comboEditor_itemStateChanged(e);
                                    }
                                 });
      }
   
      public Component getTableCellEditorComponent(JTable table, Object value, boolean isSelected, int row, int column)
      {
      //return the component to control the editing
         if (value!=null)
         {
            SKProp prop = (SKProp)value;
            switch ( prop.editorCode )
            {
               case 1:
                  {
                     thisEditor = 1;
                     prop.setComboBoxData(comboEditor);
                     return comboEditor;
                  }
            }
         }
      
      //If all else fails
         thisEditor = 0;
         return super.getTableCellEditorComponent(table,value,isSelected,row,column);
      }
   
      public Object getCellEditorValue()
      {
      //Get the value from the correct editor (last one used)
         switch (thisEditor)
         {
            case 1: 
               return comboEditor.getSelectedItem();
         }
      
      //if all else fails
         return super.getCellEditorValue();
      }
   
      public void comboEditor_itemStateChanged(ItemEvent e)
      {
         fireEditingStopped();
      }
   }