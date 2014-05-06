/**
 * Title:        SKPropFactory
 * Description:  Since SKProps get destroyed/created ALOT, to save time and
 *               reduce the number of memory allocations, always get and free your
 *               SKProps with this
 */
   package sketcher;

   import java.awt.Color;

   public class SKPropFactory
   {
      SKPropArray availableProps      = new SKPropArray(8);
   //SKPropArray availableComboProps = new SKPropArray(5);
   
      public SKPropFactory()
      {
      }
   
   //Used to return a usable Prop (from caller's perspective, same are creating new)
      public SKProp getNewProp(String name, Object data, int type)
      {
         if (availableProps.size()==0)
         { //Must create a Prop
            return new SKProp(name,data,type);
         }
         else
         {
            SKProp prop = availableProps.get( availableProps.size()-1 );
            availableProps.remove( availableProps.size()-1 );
         
            prop.PropName=name;
            prop.PropData=data;
            prop.PropType=type;
            prop.isEditable=true;
            prop.foregroundColor=Color.black;
            prop.editorCode=0;
         
            return prop;
         }
      }
   
      public SKProp getNewProp(String name, Object data, int type, boolean editable)
      {
         SKProp prop = getNewProp(name,data,type);
         prop.isEditable = editable;
         return prop;
      }
   
      public SKProp getNewProp(String name, Object data, int type, Color foreColor)
      {
         SKProp prop = getNewProp(name,data,type);
         prop.foregroundColor = foreColor;
         return prop;
      }
   
   //Used instead of "new SKComboShapeProp()"
      public SKProp getNewComboProp(String name, int dataIndex, int type, String[] comboData)
      {
         SKProp prop = getNewProp(name, comboData[dataIndex], type);
         prop.cmbData=comboData;
         prop.editorCode=1;
      
         return prop;
      }
   
      public SKProp getNewComboProp(String name, int dataIndex, int type, boolean editable, String[] comboData)
      {
         SKProp prop = getNewComboProp(name,dataIndex,type,comboData);
         prop.isEditable=editable;
      
         return prop;
      }
   
   //Used when you are done with the Prop to put it back in the recycle pool
      public void freeProp(SKProp sh)
      {
         availableProps.add(sh);
      }
   
   //Used to "free" multiple props
      public void freeProps(SKPropArray ary)
      {
      //Add all props to the correct available lists
         SKProp sh;
         for (int i=0; i<ary.size(); i++)
         {
            availableProps.add( ary.get(i) );
         }
      
         ary.clear();
      }
   }