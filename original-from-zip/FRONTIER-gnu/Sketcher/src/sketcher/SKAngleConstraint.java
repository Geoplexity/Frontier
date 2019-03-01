/**
 * Title:        SKAngleConstraint
 * Description:  Angle Constraint
 */
   package sketcher;

   import java.awt.Graphics;
   import java.awt.Color;
   import java.io.DataOutputStream;
   import java.io.DataInputStream;
   import java.io.IOException;

   public class SKAngleConstraint extends SKNormalConstraint
   {
      public double angle = 60;
   
      public SKAngleConstraint(SKMainFrame MainFrame, int idnum)
      {
         super(MainFrame,idnum,"Angle"+idnum,"Angle",4);
      }
   
      public void updateMainProperties(SKPropArray vPropList)
      {
         super.updateMainProperties(vPropList);
      
         vPropList.add(frameMain.propFactory.getNewProp("Angle", Double.toString(angle),0));
      }
   
      public boolean setMainData(String PropName, Object PropData)
      {
         if (super.setMainData(PropName,PropData)) 
            return true;
      
         if (PropName=="Angle")
         {
            angle = new Double(PropData.toString()).doubleValue();
         }
         else
            return false;
      
         return true;
      }
   
      public void doPaint(Graphics g)
      {
         g.setColor(Color.black);
         super.doPaint(g);
      }
   
      public void drawConstraint(Graphics g,int x1,int y1,int x2,int y2)
      {
         super.drawConstraint(g,x1,y1,x2,y2);
         if (frameMain.panelShapeArea.DrawConstraints && frameMain.drawConstrDetails)
            g.drawString( Double.toString(angle), (java.lang.Math.abs(x1-x2)/2 + java.lang.Math.min(x1,x2))-15,(java.lang.Math.abs(y1-y2)/2 + java.lang.Math.min(y1,y2)) );
      }
   
      public static boolean isAvailable(SKShapeArray selectedShapes)
      {  boolean available;
         available = ((selectedShapes.get(0) instanceof SKLineShape) && (selectedShapes.get(1) instanceof SKLineShape));
         available = available || ((selectedShapes.get(0) instanceof SKLineShape) && (selectedShapes.get(1) instanceof SKImageShape));
         available = available || ((selectedShapes.get(1) instanceof SKLineShape) && (selectedShapes.get(0) instanceof SKImageShape));
         available = available || ((selectedShapes.get(0) instanceof SKImageShape) && (selectedShapes.get(1) instanceof SKImageShape));
         return available;
        // return ( (selectedShapes.get(0) instanceof SKLineShape) && (selectedShapes.get(1) instanceof SKLineShape) );
      }
   
      public void writeAdditionalProps(DataOutputStream p) throws IOException
      {
         super.writeAdditionalProps(p);
      
         p.writeDouble( angle );
      }
   
      public void readAdditionalProps(DataInputStream p) throws IOException
      {
         super.readAdditionalProps(p);
      
         angle = p.readDouble();
      }
   }
