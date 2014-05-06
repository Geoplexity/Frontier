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
         super.doPaint(g);
      }
   
   
      public void drawConstraint(Graphics g,int x1,int y1,int x2,int y2)
      
      {
         //super.drawConstraint(g,x1,y1,x2,y2);
         super.drawConstraint(g,x1,y1,x2,y2);
         SKLineShape line1=(SKLineShape)ShapeList.get(0), line2=(SKLineShape)ShapeList.get(1);
         SKPointShape closePoint1=null, closePoint2=null;
         int centerx1=centerx(line1), centery1=centery(line1), centerx2=centerx(line2), centery2=centery(line2);
	 if(SKOptions.byteOptions[SKOptions.simpleSolverMode]>0)
         drawExtension(g,line1,line2);
      
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
      public void drawExtension(Graphics g, SKLineShape line1, SKLineShape line2)
      
      {
         float[] dist=new float[4];
         double[] array=new double[2];
         double[] input=new double[7];
         double[] output=new double[2];
         float min;
         int num=0;
         SKPointShape pt=null,p=null;
         dist[0] = distance(line1.pt1.getShapeX(),line1.pt1.getShapeY(),line2.pt1.getShapeX(),line2.pt1.getShapeY());
         dist[1] = distance(line1.pt1.getShapeX(),line1.pt1.getShapeY(),line2.pt2.getShapeX(),line2.pt2.getShapeY());
         dist[2] = distance(line1.pt2.getShapeX(),line1.pt1.getShapeY(),line2.pt1.getShapeX(),line2.pt1.getShapeY());
         dist[3] = distance(line1.pt2.getShapeX(),line1.pt1.getShapeY(),line2.pt2.getShapeX(),line2.pt2.getShapeY());
         min = dist[0];
         for(int i=0; i<4; i++)
            if(dist[i]<min) 
            {
               min = dist[i];
               num=i;
            }
         switch (num)
         {
            case 0: p = line1.pt1;
               pt = line2.pt1;
               break;
            case 1: p=line1.pt1;
               pt=line2.pt2;
               break;
            case 2 : p=line1.pt2;
               pt=line2.pt1;
               break;
            case 3 : p=line1.pt2;
               pt = line2.pt2;
               break;
         }
         int x=pt.getShapeX(),y=pt.getShapeY();
         double  slope=Double.MAX_VALUE, pslope=Double.MAX_VALUE;
         if(line1.pt2.getShapeX()!= line1.pt1.getShapeX())
         {   pslope=((double)(line1.pt2.getShapeY()-line1.pt1.getShapeY()))/((double)(line1.pt2.getShapeX()-line1.pt1.getShapeX()));}
         slope = -1/pslope;
         double c1,c2 ;
         c1= p.getShapeY() -(pslope*p.getShapeX());
         c2= pt.getShapeY() -(slope*pt.getShapeX());
         if(lineintersection(array,pslope,slope,c1,c2))
         {x=(int)array[0];
            y=(int)array[1];}
         else {
            if(slope ==0 )
               x=p.getShapeX();
            else
               y=p.getShapeY();	
         }
         g.drawLine(pt.getShapeX(),pt.getShapeY(),(int)x,(int)y);
         if(! ( ((x<line1.pt1.getX() && x>line1.pt2.getX()) || (x>line1.pt1.getX() && x<line1.pt2.getX())) 
               && ((x<line1.pt1.getY() && x>line1.pt2.getY()) || (x>line1.pt1.getY() && x<line1.pt2.getY())) ) )
            g.drawLine(p.getShapeX(),p.getShapeY(),(int)x,(int)y);
         if (frameMain.panelShapeArea.DrawConstraints && frameMain.drawConstrDetails)
            g.drawString( "90",x-10,y-10 );
      }
   
   }
