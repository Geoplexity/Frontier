/**
 * Title:        SKBaseConstraint
 * Description:  Base class for all constraints
 */
   package sketcher;

   import java.awt.Graphics;
   import java.awt.Graphics2D;
   import java.awt.Shape;
   import java.awt.geom.QuadCurve2D;
   import java.awt.Point;
   import java.awt.Color;
   import java.io.DataOutputStream;
   import java.io.DataInputStream;
   import java.io.IOException;

   public class SKBaseConstraint implements SKItemInterface
   {
      public static SKMainFrame   frameMain;
   
      public int           ID;  //Unique Constraint ID
      public int           repID=0;
      public String        cType;
      public String        Name;
      public int           typeID;
      public SKShapeArray  ShapeList = new SKShapeArray(2);
      public int           drawMode;
      public boolean       isDrawn = true;
      public boolean       autoAddShapeProps = true;
      public boolean       isClone = false;
      public SKBaseConstraint clonedConstraint = null;
      public SKBaseConstraint3d constraint3d = null;
      private static       QuadCurve2D  curve = new QuadCurve2D.Float();
      public int           beServedBy;
   
      public SKBaseConstraint(SKMainFrame MainFrame, int idnum, String newname, String constraintType, int TypeID)
      {
         frameMain=MainFrame;
         ID=idnum;
         typeID = TypeID;
         Name=newname;
         cType=constraintType;
      }
   
   //Severs all ties (for gc)
      public void severTies()
      {
         removeAllShapes();
         ShapeList = null;
         curve = null;
      }
      public SKBaseShape getOtherShape(SKBaseShape sh)
      {
         SKBaseShape s=ShapeList.get(1);
         for(int i=0; i<ShapeList.size(); i++)
            if(!(sh.equals(ShapeList.get(i))))
               s=ShapeList.get(i);
         return s;
      }
      public void updateMainProperties(SKPropArray vPropList)
      {
         frameMain.propFactory.freeProps(vPropList);
         vPropList.add(frameMain.propFactory.getNewProp("ID", Integer.toString(ID),0,false));
         vPropList.add(frameMain.propFactory.getNewProp("Type",cType,0,false));
         vPropList.add(frameMain.propFactory.getNewProp("Name",Name,0));
      
         if (autoAddShapeProps)
         { //Get properties for shapes
            for (int i=0; i<ShapeList.size(); i++)
            {
               ShapeList.get(i).updateConstraintProps(this,vPropList);
            }
         }
      }
   
      public boolean setMainData(String PropName, Object PropData)
      {
         if (PropName=="Name")
         {
            Name = (String)PropData;
         }
         else
            return false;
      
         return true;
      }
   
   //Called by shape when adding props to contraint for info
      public int getConInfo(SKBaseShape sh, int code)
      {
         return 0;
      }
   
   //Called by primary shape, actually paints constraint lines
      public void doPaint(Graphics g)
      {
         if (!isDrawn && !(this instanceof SKParallelConstraint) &&!(this instanceof SKPerpConstraint) && !(this instanceof SKAngleConstraint)) 
            return;
      	 System.out.println("111 const "+this+" "+ShapeList.size());
         SKBaseShape sh1 = ShapeList.get(0);
         SKBaseShape sh2 = ShapeList.get(1);
         SKBaseShape sh3 = null;
      
         Point pt1 = sh1.getConstraintPoint(this,drawMode),
         pt2 = sh2.getConstraintPoint(this,drawMode),
         pt3 = null;
         if (!frameMain.editingShape && this==frameMain.SelectedConstraint)
            g.setColor(Color.red);
         else  if (this==frameMain.mouseOverItem)
            g.setColor(Color.white);
         else
            g.setColor(Color.black);
      
         if(ShapeList.size()==3)
         {	sh3 = ShapeList.get(2);
            pt3 = sh3.getConstraintPoint(this,drawMode);
            drawConstraint(g,pt1.x,pt1.y,pt2.x,pt2.y,pt3.x,pt3.y);
         }
         else
            drawConstraint(g,pt1.x,pt1.y,pt2.x,pt2.y);
      }
   
      public void drawConstraint(Graphics g,int x1,int y1,int x2,int y2, int x3, int y3)
      {
         if (!frameMain.editingShape && this==frameMain.SelectedConstraint)
            g.setColor(Color.red);
         else  if (this==frameMain.mouseOverItem)
            g.setColor(Color.white);
         else
            g.setColor(Color.black);
      }   
   
      public void drawConstraint(Graphics g,int x1,int y1,int x2,int y2)
      {
         if (!frameMain.editingShape && this==frameMain.SelectedConstraint)
            g.setColor(Color.red);
         else  if (this==frameMain.mouseOverItem)
            g.setColor(Color.white);
         else
            g.setColor(Color.black);
      }
      /*
         if(this instanceof SKDistanceConstraint)
         { //Draw line between first and second shape
            boolean arbit1=true, arbit2=true;
            if( (((SKDistanceConstraint)this).perp))
            {
               double slope1=0,c1=0,slope2=0,c2=0;
            
               for(int i=0; i<ShapeList.size(); i++)
                  for(int j=0; j<ShapeList.get(i).getNumSubShapes(); j++)
                  {
                     SKBaseShape s = ShapeList.get(i).getSubShape(j);
                     arbit1 = arbit1 && ( !((s.getShapeX() == x1) && (s.getShapeY() == y1)) );
                     arbit2 = arbit2 && ( !((s.getShapeX() == x2) && (s.getShapeY() == y2)) );
                  }
               if( arbit1 )
               {
                  SKLineShape sh1=null;
                  SKImageShape Sh1=null;
                  if(ShapeList.get(0) instanceof SKLineShape)
                     sh1=(SKLineShape)ShapeList.get(0);
                  else if (ShapeList.get(0) instanceof SKImageShape)
                     Sh1=(SKImageShape)ShapeList.get(0);
               
               
                  if(ShapeList.get(0) instanceof SKLineShape)
                  {
                     slope1= (double)(sh1.pt2.getShapeY() - sh1.pt1.getShapeY()) / (sh1.pt2.getShapeX() - sh1.pt1.getShapeX());
                     c1 = sh1.pt2.getShapeY()  -(slope1*sh1.pt2.getShapeX());
                     slope2 = -1/slope1;
                     c2 = (y2  -(slope2*x2));
                  }
                  else if (ShapeList.get(0) instanceof SKImageShape)
                  {	slope1= (double)(Sh1.pt2.getShapeY() - Sh1.pt1.getShapeY()) / (Sh1.pt2.getShapeX() - Sh1.pt1.getShapeX());
                     c1 = Sh1.pt2.getShapeY()  -(slope1*Sh1.pt2.getShapeX());
                     slope2 = -1/slope1;
                     c2 = (y2  -(slope2*x2));
                  }
               
                  double[] array = new double[4];
                  //System.out.println("slope "+slope1+" "+((SKLineShape)sh1).pt2.getShapeY()+" "+((SKLineShape)sh1).pt1.getShapeY()+" "+((SKLineShape)sh1).pt2.getShapeX()+" "+((SKLineShape)sh1).pt1.getShapeX());
                  if(lineintersection(array,slope1,slope2,c1,c2))
                  {
                     x1=(int)array[0];
                     y1=(int)array[1];}
                  else {
                     if(slope1 ==0 )
                        x1=x2;
                     else if(slope2 ==0)
                        y1=y2;	
                  }
               }
               
               else if ( arbit2 )
               {
                  SKLineShape sh2=null;
                  SKImageShape Sh2=null;
                  if (ShapeList.get(1) instanceof SKLineShape)
                     sh2=(SKLineShape)ShapeList.get(1);	
                  else if  (ShapeList.get(1) instanceof SKImageShape)
                     Sh2=(SKImageShape)ShapeList.get(1);
                  if(ShapeList.get(1) instanceof SKLineShape)
                  {
                     slope1 = (double)(sh2.pt2.getShapeY() - sh2.pt1.getShapeY()) / (sh2.pt2.getShapeX() - sh2.pt1.getShapeX());
                     c1 = sh2.pt2.getShapeY()  -(slope1*sh2.pt2.getShapeX());
                     slope2=-1/slope1;
                     c2 = (y1  -(slope2*x1));
                  }
                  else if(ShapeList.get(1) instanceof SKImageShape)
                  {
                     slope1 = (double)(Sh2.pt2.getShapeY() - Sh2.pt1.getShapeY()) / (Sh2.pt2.getShapeX() - Sh2.pt1.getShapeX());
                     c1 = Sh2.pt2.getShapeY()  -(slope1*Sh2.pt2.getShapeX());
                     slope2=-1/slope1;
                     c2 = (y1  -(slope2*x1));
                  }
                  double[] array = new double[4];
                  //System.out.println("slope "+slope1);
                  if(lineintersection(array,slope1,slope2,c1,c2))
                  {
                     x2=(int)array[0];
                     y2=(int)array[1];}
                  else {
                     if(slope1 ==0 )
                        x2=x1;
                     else
                        y2=y1;	
                  }
               
               }
            
            }
            g.drawLine(x1,y1,x2,y2);
         }
      	
         if(this instanceof SKPerpConstraint)
         {
            SKLineShape line1=(SKLineShape)ShapeList.get(0), line2=(SKLineShape)ShapeList.get(1);
            SKPointShape closePoint1=null, closePoint2=null;
            int centerx1=centerx(line1), centery1=centery(line1), centerx2=centerx(line2), centery2=centery(line2);
            drawExtension(g,line1,line2);
         }
      	
         if(this instanceof SKParallelConstraint)
         {
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
      	
         if(this instanceof SKAngleConstraint)
         {      
            if((this instanceof SKAngleConstraint) && (SKOptions.byteOptions[ SKOptions.simpleSolverVersion ] == 0))
            {
               if((ShapeList.get(0) instanceof SKLineShape) && (ShapeList.get(1) instanceof SKLineShape)) 
               {  
                  SKLineShape line1 = (SKLineShape) ShapeList.get(0);
                  SKLineShape line2 = (SKLineShape) ShapeList.get(1);
                  double[] array=new double[2];
                  double min;
                  int num=0;
                  SKPointShape pt=line2.pt1,p=line1.pt1;
                  int x,y;
                  double  slope1=1/0.0, slope2=1/0.0;
               
                  if(line1.pt2.getShapeX()!= line1.pt1.getShapeX())
                  {   slope1=((double)(line1.pt2.getShapeY()-line1.pt1.getShapeY()))/((double)(line1.pt2.getShapeX()-line1.pt1.getShapeX()));}
               
                  if(line2.pt2.getShapeX()!= line2.pt1.getShapeX())
                  {   slope2=((double)(line2.pt2.getShapeY()-line2.pt1.getShapeY()))/((double)(line2.pt2.getShapeX()-line2.pt1.getShapeX()));}
                  double c1,c2 ;
                  c1= p.getShapeY() -(slope1*p.getShapeX());
                  c2= pt.getShapeY() -(slope2*pt.getShapeX());
                  lineintersection(array,slope1,slope2,c1,c2);
                  x=(int)array[0];
                  y=(int)array[1];
               
                  p = line1.getConstraintSubShape(this,this.getConInfo(line1,0)); 
                  pt = line2.getConstraintSubShape(this,this.getConInfo(line2,0));
               
                  g.drawLine(pt.getShapeX(),pt.getShapeY(),(int)x,(int)y);
                  g.drawLine(p.getShapeX(),p.getShapeY(),(int)x,(int)y);
                  g.drawArc(x,y,20,20,(int)java.lang.Math.atan(slope1),(int)java.lang.Math.atan(slope2));
               
                  if (frameMain.panelShapeArea.DrawConstraints && frameMain.drawConstrDetails)
                     g.drawString( Double.toString(((SKAngleConstraint)this).angle), x+10, y+10);
               }
            }
            else
            {
               if(ShapeList.size() == 3)
               {
                  g.drawLine(ShapeList.get(0).getShapeX(), ShapeList.get(0).getShapeY(), ShapeList.get(1).getShapeX(), ShapeList.get(1).getShapeY());
                  g.drawLine(ShapeList.get(2).getShapeX(), ShapeList.get(2).getShapeY(), ShapeList.get(1).getShapeX(), ShapeList.get(1).getShapeY());
               }
               else if(!((ShapeList.get(0) instanceof SKImageShape)||(ShapeList.get(1) instanceof SKImageShape)))
               {
                  Point p1, p2, p3;
                  if(ShapeList.get(0) instanceof SKPointShape)
                  {	
                     p1 = ShapeList.get(0).getConstraintPoint(this,0);
                     p2 = ShapeList.get(1).getConstraintPoint(this,((SKAngleConstraint)this).config2);
                     SKBaseShape s = ((SKLineShape)ShapeList.get(1)).getConstraintSubShape(this, ((SKAngleConstraint)this).config2);
                     SKPointShape p = ((SKLineShape)ShapeList.get(1)).getOtherPoint((SKPointShape)s);
                     p3 = new Point(p.getShapeX(), p.getShapeY());
                  }
                  else
                  {
                     p1 = ShapeList.get(1).getConstraintPoint(this,0);
                     p2 = ShapeList.get(0).getConstraintPoint(this,((SKAngleConstraint)this).config1);
                     SKBaseShape s = ((SKLineShape)ShapeList.get(1)).getConstraintSubShape(this, ((SKAngleConstraint)this).config1);
                     SKPointShape p = ((SKLineShape)ShapeList.get(0)).getOtherPoint((SKPointShape)s);
                     p3 = new Point(p.getShapeX(), p.getShapeY());
                  }
               
                  g.drawLine(p1.x,p1.y,p2.x,p2.y);
                  g.drawLine(p2.x,p2.y,p3.x,p3.y);
               
               }
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
      
      }
   */
      public boolean lineintersection( double array[], double slope1, double slope2, double c1, double c2 )
      { 
         array[0] = (c2 - c1) / (slope1 - slope2);
         array[1] = ( (slope1*c2) - (slope2*c1) ) / ( slope1 - slope2);
         if((slope1==0) || (slope2==0))
            return false;
         else 
            return true;
      }
      public float distance(int x1, int y1, int x2, int y2)
      {
         return (float)java.lang.Math.sqrt(java.lang.Math.pow((double)(x1-x2),2)+java.lang.Math.pow((double)(y1-y2),2));
      }
      public void repaint()
      {
         frameMain.RefreshShapeArea();
      }
   
      public void addShape(SKBaseShape sh)
      {
         ShapeList.add(sh);
      
      //Add to Shape's ConstraintList
         sh.ConstraintList.add(this);
      }
   
      public void removeShape(SKBaseShape sh)
      {
         if (ShapeList.size()<3)
         {
            removeAllShapes();
         }
         else
         {
            ShapeList.removeShape(sh);
         
         //Remove from that shape's ConstraintList
            sh.ConstraintList.remove(this);
         }
      }
   
      public void removeAllShapes()
      {
         for (int i=0; i<ShapeList.size(); i++)
            ShapeList.get(i).ConstraintList.remove(this);
      
         ShapeList.clear();
         frameMain.DrawnItems.remove(this);
      }
   
      public boolean isPrimaryShape(SKBaseShape sh)
      {
         return (ShapeList.get(0)==sh);
      }
   
      public boolean doHitTest(int x, int y)
      {
         //return curve.intersects(x-3,y-3,6,6);
         return false;
      }
   
   //Makes sure this constraint is available with the given selectedShapes
      public static boolean isAvailable(SKShapeArray selectedShapes)
      {
         return true;
      }
   
      public boolean constrEquals(SKShapeArray selectedShapes, SKBaseConstraint con)
      {
         for (int i=0; i<selectedShapes.size(); i++)
            if (con.ShapeList.indexOf(selectedShapes.get(i)) == -1) 
               return false;
      
         return true;
      }
   
      public String toString()
      {
         return Name;
      }
   
      public void writeToStream(DataOutputStream p, boolean writeID) throws IOException
      {
         p.writeInt(typeID);
         if (writeID) 
         { p.writeInt(ID);
            p.writeUTF(Name);
         }
         else
         {
            repID = frameMain.conI;
            p.writeInt(repID);
            frameMain.repConIDs[frameMain.conI++]= ID;
            if(this instanceof SKDistanceConstraint)
               p.writeUTF("Distance"+ID);
            else if(this instanceof SKIncidenceConstraint)
               p.writeUTF("Incidence"+ID);
            else if(this instanceof SKParallelConstraint)
               p.writeUTF("Parallel"+ID);
            else if(this instanceof SKPerpConstraint)
               p.writeUTF("Perpendicular"+ID);
            else if(this instanceof SKAngleConstraint)
               p.writeUTF("Angle"+ID);
         }
         writeAdditionalProps(p);
      
      //Write associated ShapeIDs
         p.writeInt( ShapeList.size() );
         for (int i=0; i<ShapeList.size(); i++)
         {
            if(writeID)
               p.writeInt( ShapeList.get(i).ID );
            else
               for(int j=0;  j<frameMain.repShIDs.length ; j++)
                  if(frameMain.repShIDs[j] == ShapeList.get(i).ID)
                     p.writeInt( frameMain.repShIDs[j] );
         }
      }
   
      public void writeToStream(DataOutputStream p, boolean writeID, SKShapeArray shapes) throws IOException
      { //Make sure all associated shapes are in the shapes array
         if (shapes != null)
         {
            for (int i=0; i<ShapeList.size(); i++)
            {
               if ( shapes.indexOf( ShapeList.get(i) ) == -1 ) 
                  return;
            }
         }
      
         writeToStream(p,writeID);
      }
   
      public void writeAdditionalProps(DataOutputStream p) throws IOException
      {  }
   
      public void readFromStream(DataInputStream p) throws IOException
      {
         Name = p.readUTF();
      
         readAdditionalProps(p);
      }
   
      public void readAdditionalProps(DataInputStream p) throws IOException
      {  }
   
      public SKItemInterface getClonedItem()
      {
         return null;
      }
   
   
      public void cloneConstraint(SKBaseConstraint con, SKShapeArray shapeArray)
      {
         con.Name = Name;
         con.isClone=true;
         con.clonedConstraint = this;
         con.ID = ID;
         for(int i=0;i<ShapeList.size(); i++)
         {
            SKBaseShape cloneShape = null;
            for(int j =0; j<shapeArray.size(); j++)
            {
               if(cloneShape==null)
                  for(int k=0; k<shapeArray.get(j).getNumSubShapes()+1; k++)
                  {  
                     //System.out.println("111 "+shapeArray.get(j).Name+"'s "+shapeArray.get(j).getSubShape(k)+"'s clone "+shapeArray.get(j).getSubShape(k).clonedShape+" matched with "+ShapeList.get(i).Name);
                     if(shapeArray.get(j).getSubShape(k).ID==ShapeList.get(i).ID) 
                     {cloneShape = shapeArray.get(j).getSubShape(k);
                        break;}
                  }
	      
	       //cloneShape = shapeArray.findByID(ShapeList.get(i).ID);
            }
            if(cloneShape!=null)
            {
               con.ShapeList.add(cloneShape);
               cloneShape.ConstraintList.add(con);
	       //System.out.println("111 con = "+con+" shapeList = "+con.ShapeList.size()+" cloneShape = "+cloneShape+" conList = "+cloneShape.ConstraintList.size());
            }
         }
      }
   }

