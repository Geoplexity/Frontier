/**
 * Title:        SKDistanceConstraint
 * Description:  Distance Constraint
 */
   package sketcher;

   import java.awt.Graphics;
   import java.awt.Color;
   import java.awt.geom.Line2D;
   import java.awt.Point;
   import java.io.DataOutputStream;
   import java.io.DataInputStream;
   import java.io.IOException;
   import javax.swing.JLabel;

   public class SKDistanceConstraint extends SKNormalConstraint
   {
      public static Line2D.Double line = new Line2D.Double();
      public double distance = 1;
      public boolean perp = false;
   
      public SKDistanceConstraint(SKMainFrame MainFrame, int idnum)
      {
         super(MainFrame,idnum,"Distance"+idnum,"Distance",0);
      }
   
      public void updateMainProperties(SKPropArray vPropList)
      {
         super.updateMainProperties(vPropList);
      
         vPropList.add(frameMain.propFactory.getNewProp("Distance", Double.toString(distance),0));
      }
   
      public boolean setMainData(String PropName, Object PropData)
      {
         if (super.setMainData(PropName,PropData))  
            return true;
      
         if (PropName=="Distance")
         {
            distance = new Double(PropData.toString()).doubleValue();
         }
         else 
            return false;
      
         return true;
      }
   
      public void doPaint(Graphics g)
      {
         if (!isDrawn)  
            return;
         SKBaseShape sh1 = ShapeList.get(0);
         SKBaseShape sh2 = ShapeList.get(1);
      
         Point pt1 = sh1.getConstraintPoint(this,config1),
         pt2 = sh2.getConstraintPoint(this,config2);
      
         drawConstraint(g,pt1.x,pt1.y,pt2.x,pt2.y);
      }
   
      public void drawConstraint(Graphics g,int x1,int y1,int x2,int y2)
      {
         super.drawConstraint(g,x1,y1,x2,y2);
      //Draw value
         if (frameMain.panelShapeArea.DrawConstraints && frameMain.drawConstrDetails)
            g.drawString( Double.toString(distance), (java.lang.Math.abs(x1-x2)/2 + java.lang.Math.min(x1,x2))-10,(java.lang.Math.abs(y1-y2)/2 + java.lang.Math.min(y1,y2)) );
      }
   
      public void repaint()
      {
         frameMain.RefreshShapeArea( (int)java.lang.Math.min(line.x1,line.x2)-1,(int)java.lang.Math.min(line.y1,line.y2)-1,
                                 (int)java.lang.Math.abs(line.x1-line.x2)+1,(int)java.lang.Math.abs(line.y1-line.y2)+1 );
      }
   
      public boolean doHitTest(int x, int y)
      {
         //System.out.println("dist "+ShapeList.size());
         Point pt1 = ShapeList.get(0).getConstraintPoint(this,config1),
         pt2 = ShapeList.get(1).getConstraintPoint(this,config2);
      
         line.setLine(pt1.x,pt1.y,pt2.x,pt2.y);
      
         return line.intersects(x-3,y-3,6,6);
      }
   
      public void writeAdditionalProps(DataOutputStream p) throws IOException
      {
         super.writeAdditionalProps(p);
      
         p.writeDouble( distance );
      }
   
      public void readAdditionalProps(DataInputStream p) throws IOException
      {
         super.readAdditionalProps(p);
      
         distance = p.readDouble();
      }
   }
