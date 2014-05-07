/**
 * Title:        SKRectShape
 * Description:  Rectangle shape
 */
   package sketcher;

   import java.awt.Graphics;
   import java.awt.Color;

   public class SKRectShape extends SKNormalShape
   {
      public SKRectShape(SKMainFrame frame, int newID, int x, int y)
      {
         super(frame,newID,"Rect"+newID,"Rect",-1,x,y);
      
         IconPath = "/images/recticon.gif";
         reshape(x-10,y-10,20,20); //Subtract half of width and heigth so x,y is center
      }
   
      public void paintComponent(Graphics g)
      {
      //Paint shape
         g.setColor(Color.blue);
         g.fillRect(0,0,getWidth(),getHeight());
      
         if (Selected)
         { //Draw selected rect
            g.setColor(Color.red);
            g.drawRect(0,0,getWidth()-1,getHeight()-1);
         }
      }
   
      public void updateMainShapeData(SKPropArray vPropList, SKConstraintArray vCurrentConstraints)
      {
         super.updateMainShapeData(vPropList,vCurrentConstraints);
      
         if (vPropList != null)
         { //Update properties
            vPropList.add(frameMain.propFactory.getNewProp("Height",new Integer(getHeight()),1));
            vPropList.add(frameMain.propFactory.getNewProp("Width",new Integer(getWidth()),1));
         }
      }
   
      public boolean setShapeData(String PropName, Object PropData)
      {
         if (super.setShapeData(PropName,PropData))
            return true;
      
         if (PropName=="Height")
         {
            setSize(getWidth(),new Integer(PropData.toString()).intValue());
         }
         else if (PropName=="Width")
         {
            setSize(new Integer(PropData.toString()).intValue(),getHeight());
         }
         else
            return false;
      
         return true;
      }
   
   }