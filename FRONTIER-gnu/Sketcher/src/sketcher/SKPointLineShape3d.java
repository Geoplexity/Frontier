/**
 * Title:        SKPointLineShape
 * Description:  Point shape for line
 */
   package sketcher;
   import java.awt.Graphics;
   import java.awt.Graphics2D;
   import java.awt.Color;
   import java.io.DataOutputStream;
   import java.io.DataInputStream;
   import java.io.IOException;
	import javax.vecmath.*;

   public class SKPointLineShape3d extends SKPointSubShape3d
   {
      public int         pointType = 0; //0=point, 1=infinite
      public SKMainFrame Frame = null;
   
      public SKPointLineShape3d(SKMainFrame frame, int id, SKLineShape3d line, int x, int y)
      {
         super(frame,id,x,y,line);
         Frame = frame;
      }
   
      public void paintConstraints(Graphics g)
      { //Divert constraint drawing to the line (and only do it once per line)
         if ( ((SKLineShape3d)parentShape).getLeftPoint().equals(this))
            ((SKLineShape3d)parentShape).paintConstraints(g);
      }
   
      public void paintComponent(Graphics g)
      {Color red = Color.red;
      //Paint shape
         switch (pointType)
         {
            case 0:  super.paintComponent(g);
               break;
            case 1: 
               {
                  //updateColors();
                  SKLineShape3d thisLine = (SKLineShape3d)parentShape;
               
                  SKPointShape3d otherPt = thisLine.getOtherPoint(this);
                  float x1 = getShapeX(),
                  y1 = getShapeY(),
                  w1 = getWidth(),
                  h1 = getHeight()>>1,
                  x2 = otherPt.getShapeX(),
                  y2 = otherPt.getShapeY(),
                  slope = (y2-y1)/(x2-x1);
                  g.setColor(red);
               }
         }
      
      }
   
      public void setPointType(int newType)
      {
         if (newType != pointType)
         {
            pointType = newType;
            repaint();
         }
      }
   
      public void writeAdditionalProps(DataOutputStream p) throws IOException
      {
         super.writeAdditionalProps(p);
      
         p.writeInt(pointType);
      }
   
      public void readAdditionalProps(DataInputStream p) throws IOException
      {
         super.readAdditionalProps(p);
      
         pointType = p.readInt();
      }
      public void setShapePoint(Point3d pt)
      {
         super.setShapePoint(pt);
         parentShape.doMove();
      }
   }
