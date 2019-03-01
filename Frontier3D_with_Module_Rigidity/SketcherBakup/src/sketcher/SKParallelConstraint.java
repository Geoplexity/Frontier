/**
 * Title:        SKParallelConstraint
 * Description:  Parallel Constraint
 */
   package sketcher;
   import java.awt.Graphics;
   import java.awt.Color;

   public class SKParallelConstraint extends SKBaseConstraint
   {
      public int count;
      public SKMainFrame  frame= null;
      public SKParallelConstraint(SKMainFrame frameMain, int idnum)
      {
         super(frameMain,idnum,"Parallel"+idnum,"Parallel",3);
         frame = frameMain;
         autoAddShapeProps = false;
         drawMode = 1;
      }
   
      public boolean setMainData(String PropName, Object PropData)
      {
         return super.setMainData(PropName,PropData);
      }
   
      public static boolean isAvailable(SKShapeArray selectedShapes)
      {
         return ( (selectedShapes.get(0) instanceof SKLineShape) && (selectedShapes.get(1) instanceof SKLineShape) );
      }
   
   
      public void doPaint(Graphics g)
      
      {
         this.isDrawn=false;
         super.doPaint(g);
      }
   
   
      public void drawConstraint(Graphics g,int x1,int y1,int x2,int y2)
      
      
      { //super.drawConstraint(g,x1,y1,x2,y2);
         super.drawConstraint(g,x1,y1,x2,y2);
         SKLineShape line1 = (SKLineShape) ShapeList.get(0);
         SKLineShape line2 = (SKLineShape) ShapeList.get(1);
         int centerx1, centery1, centerx2, centery2, x11, x12, x21, x22, y11, y12, y21, y22;
           //float slope = Float.MAX_VALUE,dslope ; 
         float slope = (float)(1/0.0), dslope;
      
         if(line1.pt1.getX() != line1.pt2.getX())
            slope = (line1.pt1.getY()-line1.pt2.getY()) / (line1.pt1.getX()-line1.pt2.getX()); 
         dslope = -1/slope;
         SKBaseConstraint cons1 =  null, cons2 = null;
      
         if(line1.pt1.getX()>line1.pt1.getX()) 
            centerx1 = (int) (line1.pt2.getShapeX() + ((line1.pt1.getShapeX()-line1.pt2.getShapeX())/2));
         else  
            centerx1 = (int) (line1.pt1.getShapeX() + ((line1.pt2.getShapeX()-line1.pt1.getShapeX())/2));
         if(line2.pt1.getX()>line2.pt1.getX()) 
            centerx2 = (int) (line2.pt2.getShapeX() + ((line2.pt1.getShapeX()-line2.pt2.getShapeX())/2));
         else  
            centerx2 = (int) (line2.pt1.getShapeX() + ((line2.pt2.getShapeX()-line2.pt1.getShapeX())/2));
         if(line1.pt1.getY()>line1.pt1.getY()) 
            centery1 = (int) (line1.pt2.getShapeY() + ((line1.pt1.getShapeY()-line1.pt2.getShapeY())/2));
         else  
            centery1 = (int) (line1.pt1.getShapeY() + ((line1.pt2.getShapeY()-line1.pt1.getShapeY())/2));
         if(line2.pt1.getY()>line2.pt1.getY()) 
            centery2 = (int) (line2.pt2.getShapeY() + ((line2.pt1.getShapeY()-line2.pt2.getShapeY())/2));
         else  
            centery2 = (int) (line2.pt1.getShapeY() + ((line2.pt2.getShapeY()-line2.pt1.getShapeY())/2));
         int sense = -1,d=1;
         for(int i=0; i<((SKParallelConstraint)this).count ; i++)
         {
            int[] in = new int[2];
            int[] out = new int[2];
         
            extPoint(centerx1,centery1,slope,sense,d,in);
            extPoint(in[0],in[1],dslope,-1,1,out);
            in[0]=out[0];
            in[1]=out[1];
            extPoint(in[0],in[1],dslope,1,2,out);
            x11 = in[0];
            y11 = in[1];
            x12 = out[0];
            y12 = out[1];
         
            extPoint(centerx2,centery2,slope,sense,d,in);
            extPoint(in[0],in[1],dslope,-1,1,out);
            in[0]=out[0];
            in[1]=out[1];
            extPoint(in[0],in[1],dslope,1,2,out);
            x21 = in[0];
            y21 = in[1];
            x22 = out[0];
            y22 = out[1];
            if (frameMain.panelShapeArea.DrawConstraints && frameMain.drawConstrDetails)
            {
               g.drawLine(x11,y11,x12,y12);
               g.drawLine(x21,y21,x22,y22);
            }
            sense = -sense;
            d=d+2;
         }
      }
      public void extPoint(int x,int y, float slope, int direction, int dist, int[] out)
      
      {
         int temp=0,inc=1;
         float tslope = -1/slope;
         boolean gox=true;
         while(temp<dist)
         {
            if(gox && tslope!=0.0)
            {
               out[0] =  x + (int)(direction*inc);
               out[1] =  y + (int)(slope * (out[0]-x));
            }
            else 
            {
               out[1] =  y + (int)(direction*inc);
               out[0] =  x + (int)((out[1]-y) / slope);
            }
            if(temp == (int)distance(x,y,out[0],out[1]))
               gox=false;
            else temp = (int)distance(x,y,out[0],out[1]);
            inc++;
         }
      }
   }