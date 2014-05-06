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

   public class SKPointLineShape extends SKPointSubShape
   {
      public int         pointType = 0; //0=point, 1=infinite
      public SKMainFrame Frame = null;
   
      public SKPointLineShape(SKMainFrame frame, int id, SKLineShape line, int x, int y)
      {
         super(frame,id,x,y,line);
         Frame = frame;
      }
      public void cloneShape(SKBaseShape sh)
      {
         super.cloneShape(sh);
         ((SKPointLineShape)sh).pointType = pointType;
      }
      public void paintConstraints(Graphics g)
      { //Divert constraint drawing to the line (and only do it once per line)
         if ( ((SKLineShape)parentShape).getLeftPoint().equals(this))
            ((SKLineShape)parentShape).paintConstraints(g);
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
                  updateColors();
                  g.setColor(bodyColor);
               
                  SKLineShape thisLine = (SKLineShape)parentShape;
               
                  SKPointShape otherPt = thisLine.getOtherPoint(this);
                  float x1 = getShapeX(),
                  y1 = getShapeY(),
                  w1 = getWidth(),
                  h1 = getHeight()>>1,
                  x2 = otherPt.getShapeX(),
                  y2 = otherPt.getShapeY(),
                  slope = (y2-y1)/(x2-x1);
                  g.setColor(red);
                 /* if(x1>x2)
                  {              
                     while(x1<(Frame.getWidth()-x2))
                     {x1++;
                        y1 = y2 - (slope*(x2-x1));
                     }
                     g.drawLine((int)x1,(int)y1,(int)x2,(int)y2);
                     //g.fillPolygon(new int[] {5,(int)w1>>1,(int)w1-5}, new int[] {(int)h1,0,(int)h1}, 3);
                     System.out.println("x1 "+(int)x1+" y1 "+(int)y1+" x2 "+(int)x2+" y2 "+(int)y2);  
                  }
                  else if(x1<x2)
                  {                     
                     while(x1>0)
                     {x1--;
                        y1 = y2 - (slope*(x2-x1));}
                     g.drawLine((int)x1,(int)y1,(int)x2,(int)y2);
                  }
                  else
                  {
                     if(y1>y2)
                     {                     
                        while(y1<(Frame.getHeight()-y2))
                        {y1++;
                           x1 = x2 - ((y2-y1)/slope);}
                        g.drawLine((int)x1,(int)y1,(int)x2,(int)y2);
                     }
                     else if(y1<y2)
                     {                     
                        while(y1>0)
                        {y1--;
                           x1 = x2 - ((y2-y1)/slope);}
                        g.drawLine((int)x1,(int)y1,(int)x2,(int)y2);
                     }
                  }
               
               
                  if (x1 != x2)
                  {
                     Graphics2D g2d = (Graphics2D)g;
                     g2d.rotate( java.lang.Math.atan( (y1-y2)/(x1-x2) )-java.lang.Math.PI/2 ,w1/2,h1/2);
                     if (this == thisLine.getLeftPoint())
                        g.fillPolygon(new int[] {5,(int)w1>>1,(int)w1-5}, new int[] {(int)h1,0,(int)h1}, 3);
                     else
                        g.fillPolygon(new int[] {5,(int)w1>>1,(int)w1-5}, new int[] {0,(int)h1,0}, 3);
                  }
                  else
                  {
                     if (this == thisLine.getTopPoint())
                        g.fillPolygon(new int[] {5,(int)w1>>1,(int)w1-5}, new int[] {(int)h1,0,(int)h1}, 3);
                     else
                        g.fillPolygon(new int[] {5,(int)w1>>1,(int)w1-5}, new int[] {0,(int)h1,0}, 3);
                  }
               
                 //g.drawPolyline(new int[] {4*(x1-x2)+3*(x2-x1),w1/2,-3*(x1-x2)-4*(x2-x1)},new int[] {4*(y1-y2)+3*(y2-y1),h1/2,-3*(y1-y2)-4*(y2-y1)}, 3);
                 //g.drawPolyline(new int[] {4*(x1-x2)-3*(x2-x1),w1/2},new int[] {4*(y1-y2)+3*(y2-y1),h1/2}, 2);
               
                 if (y1 != y2)
                 {
                   float  slope    = (y1-y2)/(x1-x2),
                          newslope = -slope/2,
                          newb     = y1-(newslope*x1),
                          newx     = (float)java.lang.Math.sqrt( 25/((1/(newslope*newslope) + 1)*(newslope*newslope)) ) + x1,
                          newy     = newslope*newx+newb;
               
                   g.drawLine((int)newx,(int)newy,(int)w1>>1,(int)h1>>1);
                 }
                 */
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
      public void readAdditionalProps(DataInputStream p,int x, int y) throws IOException
      
      {
         super.readAdditionalProps(p,x,y);
      
         pointType = p.readInt();
      }
   
   }