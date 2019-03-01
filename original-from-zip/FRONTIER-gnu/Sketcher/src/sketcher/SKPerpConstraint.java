/**
 * Title:        SKPerpConstraint
 * Description:  Perpendicular constraint
 */
   package sketcher;

   import java.awt.Graphics;
   import java.awt.Color;

   public class SKPerpConstraint extends SKBaseConstraint
   {
      public SKPerpConstraint(SKMainFrame frameMain, int idnum)
      {
         super(frameMain,idnum,"Perpendicular"+idnum,"Perpendicular",2);
      
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
         g.setColor(Color.black);
         super.doPaint(g);
      }
   
   
      public void drawConstraint(Graphics g,int x1,int y1,int x2,int y2)
      
      {
         super.drawConstraint(g,x1,y1,x2,y2);
         /*SKLineShape line1=(SKLineShape)ShapeList.get(0), line2=(SKLineShape)ShapeList.get(1);
         SKPointShape closePoint1=null, closePoint2=null;
         int centerx1=centerx(line1), centery1=centery(line1), centerx2=centerx(line2), centery2=centery(line2);
         drawExtension(g,line1,line2);*/
      
         if (frameMain.panelShapeArea.DrawConstraints && frameMain.drawConstrDetails)
            g.drawString( "90", (java.lang.Math.abs(x1-x2)/2 + java.lang.Math.min(x1,x2))-15,(java.lang.Math.abs(y1-y2)/2 + java.lang.Math.min(y1,y2)) );
      }
   
      public float distance(int x1, int y1, int x2, int y2)
      {
         return (float)java.lang.Math.sqrt(java.lang.Math.pow((double)(x1-x2),2)+java.lang.Math.pow((double)(y1-y2),2));
      }
      public int centerx(SKLineShape line)
      {
         if(line.pt1.getShapeX() > line.pt2.getShapeX())
            return (line.pt2.getShapeX() + (line.pt1.getShapeX()-line.pt2.getShapeX())/2);
         else 
            return (line.pt1.getShapeX() + (line.pt2.getShapeX()-line.pt1.getShapeX())/2);
      }
   
      public int centery(SKLineShape line)
      
      {
         if(line.pt1.getShapeY() > line.pt2.getShapeY())
            return (line.pt2.getShapeY() + (line.pt1.getShapeY()-line.pt2.getShapeY())/2);
         else 
            return (line.pt1.getShapeY() + (line.pt2.getShapeY()-line.pt1.getShapeY())/2);
      }
      /*public void drawExtension(Graphics g, SKLineShape line1, SKLineShape line2)
      {
         SKPointShape pt=null, p=null;
         if(distance(line1.getShapeX(),line1.getShapeY(),line2.pt1.getShapeX(),line2.pt1.getShapeY()) > distance(line1.getShapeX(),line1.getShapeY(),line2.pt2.getShapeX(),line2.pt2.getShapeY()))
            pt = line2.pt2;
         else if(distance(line1.getShapeX(),line1.getShapeY(),line2.pt1.getShapeX(),line2.pt1.getShapeY()) < distance(line1.getShapeX(),line1.getShapeY(),line2.pt2.getShapeX(),line2.pt2.getShapeY()))
            pt = line2.pt1;
      
         if(distance(line2.getShapeX(),line2.getShapeY(),line1.pt1.getShapeX(),line1.pt1.getShapeY()) > distance(line2.getShapeX(),line2.getShapeY(),line1.pt2.getShapeX(),line1.pt2.getShapeY()))
            p = line1.pt2;
         else if (distance(line2.getShapeX(),line2.getShapeY(),line1.pt1.getShapeX(),line1.pt1.getShapeY()) < distance(line2.getShapeX(),line2.getShapeY(),line1.pt2.getShapeX(),line1.pt2.getShapeY()))
            p = line1.pt1;
      System.out.println("line1 "+p+" line2 "+pt);
         float x=(float)pt.getShapeX(),y=(float)pt.getShapeY();
         boolean satisfies = false,ifloop = false;
         float slope=Float.MAX_VALUE;
      
         if(line2.pt2.getShapeX()!= line2.pt1.getShapeX())
            slope=(line2.pt2.getShapeY()-line2.pt1.getShapeY())/(line2.pt2.getShapeX()-line2.pt1.getShapeX());
      
         float c = y + (x/slope);
      
         while(!satisfies)
         {
            if(line2.pt1.getShapeX()!=line2.pt2.getShapeX())
            {
               if((int)x<p.getShapeX())
               {x=x+(float)0.5;
                  ifloop=true;}
               else {x=x-(float)0.5;
                  if(ifloop) satisfies = true;}
               y=(slope*(x-(float)pt.getShapeX()))+(float)pt.getShapeY();
               //if(x!=line1.pt1.getShapeX())
               float temp =   (y+(x/slope)-c); 
            
               if( ((int)java.lang.Math.floor(y+(x/slope)-c) == 0) || ((int)java.lang.Math.ceil(y+(x/slope)-c) == 0) )
                  satisfies = true;
               else if(y==L.pt1.getShapeY())
               {System.out.println("else if");
                  satisfies = true;}
            }
            else { 
               if(y<line1.getShapeY()){ y++;
                  ifloop = true;}
               else {y--;
                  if(ifloop) satisfies = true;}
               if((int)y==line1.pt1.getShapeY())
                  satisfies = true;}
         }
         g.drawLine(pt.getShapeX(),pt.getShapeY(),(int)x,(int)y);
         if( !( (x<line1.pt1.getX() && x>line1.pt2.getX()) || (x>line1.pt1.getX() && x<line1.pt2.getX()) )
            && !( (x<line1.pt1.getY() && x>line1.pt2.getY()) || (x>line1.pt1.getY() && x<line1.pt2.getY()) ) )
            g.drawLine(p.getShapeX(),p.getShapeY(),(int)x,(int)y);
      }*/
   
   }