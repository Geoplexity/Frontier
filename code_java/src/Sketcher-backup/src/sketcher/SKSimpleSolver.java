/**
 * Title:        SKSimpleSolver
 * Description:  Used for automatic solving of "simple" constraints
 */
   package sketcher;

   import java.awt.Point;
   import java.awt.geom.*;
   import java.util.*;


   public class SKSimpleSolver
   {
      static SKConstraintArray cons;
      static SKShapeArray shapes;
      static int count;
      		/**************************************************Simple Solver version1.0****************************************************/
   
   
      public static boolean DoSimpleSolver(SKBaseConstraint con, SKBaseShape preservedShape, SKConstraintArray cons)
      
      {
         if (preservedShape==null)  preservedShape = con.ShapeList.get(0);
         cons.add(con);
         boolean res = true;
         //System.out.println(con.typeID);
         switch (con.typeID)
         {
            case 1 : 
               { //Incidence
                  int i,g;
                  SKBaseShape sh;
                  for (i=0; i<con.ShapeList.size(); i++)
                     if (con.ShapeList.get(i) != preservedShape)
                     {
                        sh = con.ShapeList.get(i);
                        Point pt1,pt2;
                        g = con.getConInfo(sh,0);
                        pt1 = sh.getConstraintPoint(con,g);
                        pt2 = preservedShape.getConstraintPoint(con,con.getConInfo(preservedShape,0));
                        if (!pt1.equals( pt2 ))
                        {
                           sh.setConstraintPoint(con,g,pt2);
                           ResolveSimpleConstraints(sh,cons);
                        }
                     }
               }
               break;
            case 2: 
               { //Perpendicular
                  SKPointShape ppt2 = ((SKLineShape)preservedShape).getTopPoint(),
                  ppt1 = ((SKLineShape)preservedShape).getOtherPoint(ppt2);
                  SKPointShape spt1, spt2;
                  double s1x,s1y,s2x,s2y,sSlope,dsq;
                  double p1x = ppt1.getX(),
                  p1y = ppt1.getY(),
                  p2x = ppt2.getX(),
                  p2y = ppt2.getY(),
                  pSlope = (p1y-p2y)/(p1x-p2x);
               
                  SKBaseShape sh;
                  for (int i=0; i<con.ShapeList.size(); i++)
                  {
                     sh = con.ShapeList.get(i);
                     if (sh != preservedShape)
                     {
                        spt2 = ((SKLineShape)sh).getTopPoint();
                        spt1 = ((SKLineShape)sh).getOtherPoint(spt2);
                     
                        s1x = spt1.getX();
                        s2x = spt2.getX();
                        if (p1x!=p2x)
                        {s1y = spt1.getY();
                           s2y = spt2.getY();
                           sSlope = (s1y-s2y)/(s1x-s2x);
                        
                           if ( java.lang.Math.round(pSlope*10) != java.lang.Math.round((1/sSlope)*10))
                           {
                              dsq = ((s1x-s2x)*(s1x-s2x)) + ((s1y-s2y)*(s1y-s2y));
                              double yf = -java.lang.Math.sqrt( dsq/(1+(pSlope*pSlope)) ) + s1y,
                              xf = pSlope*(s1y-yf) + s1x;
                              int   xfi = (int)java.lang.Math.round(xf),
                              yfi = (int)java.lang.Math.round(yf);
                           
                              spt2.doMove( xfi - (int)s2x, yfi - (int)s2y, false );
                              ResolveSimpleConstraints(sh,cons);
                           }
                        }
                        else
                        { //Handle undefinded case
                           s1y = spt1.getY();
                           s2y = spt2.getY();
                           if (s1y!=s2y)
                           {
                              dsq = java.lang.Math.round(java.lang.Math.sqrt(((s1x-s2x)*(s1x-s2x)) + ((s1y-s2y)*(s1y-s2y))));
                              if (s1y<s2y)
                                 spt2.doMove( (int)(s1x+dsq-s2x), (int)s1y-(int)s2y, false );
                              else
                                 spt2.doMove( (int)(s1x-dsq-s2x), (int)s1y-(int)s2y, false );
                           
                              ResolveSimpleConstraints(sh,cons);
                           }
                        }
                     }
                  }
               }
               break;
            case 3 : 
               { //Parallel
                  SKPointShape ppt1 = ((SKLineShape)preservedShape).getLeftPoint(),
                  ppt2 = ((SKLineShape)preservedShape).getOtherPoint(ppt1);
                  SKPointShape spt1, spt2;
                  double s1x,s1y,s2x,s2y,sSlope,dsq;
                  double p1x = ppt1.getX(),
                  p1y = ppt1.getY(),
                  p2x = ppt2.getX(),
                  p2y = ppt2.getY(),
                  pSlope = (p1y-p2y)/(p1x-p2x);
               
                  SKBaseShape sh;
                  for (int i=0; i<con.ShapeList.size(); i++)
                  {
                     sh = con.ShapeList.get(i);
                     if (sh != preservedShape)
                     {
                        spt1 = ((SKLineShape)sh).getLeftPoint();
                        spt2 = ((SKLineShape)sh).getOtherPoint(spt1);
                     
                        s1x = spt1.getX();
                        s2x = spt2.getX();
                        if (p1x!=p2x)
                        {
                           s1y = spt1.getY();
                           s2y = spt2.getY();
                           sSlope = (s1y-s2y)/(s1x-s2x);
                        
                           if ( java.lang.Math.round(pSlope*10) != java.lang.Math.round(sSlope*10))
                           {
                              dsq = ((s1x-s2x)*(s1x-s2x)) + ((s1y-s2y)*(s1y-s2y));
                              double xf = java.lang.Math.sqrt( dsq/(1+(pSlope*pSlope)) ) + s1x,
                              yf = -pSlope*(s1x-xf) + s1y;
                              int   xfi = (int)java.lang.Math.round(xf),
                              yfi = (int)java.lang.Math.round(yf);
                           
                              spt2.doMove( xfi - (int)s2x, yfi - (int)s2y, false );
                              ResolveSimpleConstraints(sh,cons);
                           }
                        }
                        else
                        { //Handle undefinded case
                           if (s1x!=s2x)
                           {s1y = spt1.getY();
                              s2y = spt2.getY();
                              dsq = java.lang.Math.round(java.lang.Math.sqrt(((s1x-s2x)*(s1x-s2x)) + ((s1y-s2y)*(s1y-s2y))));
                              if (s1y<s2y)
                                 spt2.doMove( (int)s1x-(int)s2x, (int)(s1y+dsq-s2y), false );
                              else
                                 spt2.doMove( (int)s1x-(int)s2x, (int)(s1y-dsq-s2y), false );
                           
                              ResolveSimpleConstraints(sh,cons);
                           }
                        }
                     }
                  }
               }
               break;
            case 4:
               {// angle
                  if(con.ShapeList.size()<3)
                  {
                     double slope1=1/0.0,slope2=1/0.0,c1,inc1=0,angle=0,inc2=0,inco;
                     SKLineShape sh =(SKLineShape) preservedShape;
                     slope1 = (double) (sh.pt2.getShapeY() - sh.pt1.getShapeY()) /(double) (sh.pt2.getShapeX() -sh.pt1.getShapeX());
                     c1 = sh.pt1.getShapeY() - (slope1*sh.pt1.getShapeX());
                     inc1 = java.lang.Math.atan(slope1);
                     angle = ((SKAngleConstraint)con).angle;
                     SKBaseShape sh1 = getOtherShape(con,sh);
                  
                     for (int j=0; j<con.ShapeList.size(); j++)
                     {
                        sh =(SKLineShape) con.ShapeList.get(j);
                        if (sh != preservedShape)
                        {
                           SKPointShape p = sh.getTopPoint();
                           SKPointShape p1 = sh.getOtherPoint(p);
                           float dist = distance (p.getShapeX(),p.getShapeY(),p1.getShapeX(),p1.getShapeY());
                           slope2 = (double) (sh.pt2.getShapeY() - sh.pt1.getShapeY()) /(double)(sh.pt2.getShapeX()-sh.pt1.getShapeX());
                           inco = java.lang.Math.atan(slope2);
                           inc2 = java.lang.Math.toRadians(angle) + inc1;
                           int g = con.getConInfo(sh,0);
                           SKPointShape p2 = sh.getConstraintSubShape(con,g);
                           AffineTransform T = new AffineTransform();
                           T.setToRotation(inc2,p2.getX(),p2.getY());
                        
                           float[] src,dst;
                           src = new float[2];
                           dst = new float[2];             
                           src[0]=sh.getOtherPoint(p2).getShapeX();
                           src[1]=sh.getOtherPoint(p2).getShapeY();
                           T.transform(src,0,dst,0,1);
                           sh.getOtherPoint(p2).doMove(((int)dst[0]-sh.getOtherPoint(p2).getShapeX()),((int)dst[1]-sh.getOtherPoint(p2).getShapeY()),false);
                        
                           ResolveSimpleConstraints(sh,cons);
                        }
                     }
                  }
               
               }
               break;
            case 5 : 
               { //Tangent
                  if(preservedShape instanceof SKLineShape)
                  {SKCircleShape sh ;
                     SKLineShape sh1 = (SKLineShape)preservedShape;
                     double[] input = new double[8];
                     double[] output = new double[8];
                     double[] array = new double[4];
                     for (int j=0; j<con.ShapeList.size(); j++)
                     { 
                        if (con.ShapeList.get(j) != preservedShape)
                        {
                           sh =(SKCircleShape) con.ShapeList.get(j);
                           int centerx = sh.center.getShapeX();
                           int centery = sh.center.getShapeY();
                           double slope,c1,c2,pslope;
                           slope =((double) (sh1.pt2.getShapeY()-sh1.pt1.getShapeY())/(double)(sh1.pt2.getShapeX()-sh1.pt1.getShapeX()));
                           pslope = -1/slope;
                           int x,y,i=0;
                           float rad;
                           c1 = (((SKLineShape)preservedShape).pt2.getShapeY() - (slope*((SKLineShape)preservedShape).pt2.getShapeX()));
                           c2 = centery - (pslope * centerx);
                        
                           if(sh.radius == -1) rad = 20;
                           else rad = (float)sh.radius;
                           if(((SKLineShape)preservedShape).pt2.getShapeY()==((SKLineShape)preservedShape).pt1.getShapeY())
                              if (((SKLineShape)preservedShape).pt2.getShapeY()<centery)
                                 sh.doMove(0,(int)(((SKLineShape)preservedShape).pt2.getShapeY()-centery+rad),false);
                              else 
                                 sh.doMove(0,(int)(((SKLineShape)preservedShape).pt2.getShapeY()-centery-rad),false);
                           else if(((SKLineShape)preservedShape).pt2.getShapeX()==((SKLineShape)preservedShape).pt1.getShapeX())
                              if (((SKLineShape)preservedShape).pt2.getShapeX()<centerx)
                                 sh.doMove((int)(((SKLineShape)preservedShape).pt2.getShapeX()-centerx+rad),0,false);
                              else 
                                 sh.doMove((int)(((SKLineShape)preservedShape).pt2.getShapeY()-centery-rad),0,false);
                           else
                           {
                              c1 = (((SKLineShape)preservedShape).pt2.getShapeY() - (slope*((SKLineShape)preservedShape).pt2.getShapeX()));
                              c2 = centery - (pslope * centerx);
                              input[0] = centerx;
                              input[1] = centery;
                              input[2] = rad;
                              input[3] = pslope;
                              input[4] = c2;
                              input[5] = ((SKLineShape)preservedShape).getShapeX();
                              input[6] = ((SKLineShape)preservedShape).getShapeY();
                              circleintersection(input,output);
                              x=(int)output[0];
                              y=(int)output[1];
                              lineintersection(array,slope,pslope,c1,c2);
                              x = (int)array[0] - x;
                              y = (int)array[1] - y;
                              sh.doMove(x,y,false);
                           }
                           ResolveSimpleConstraints(sh,cons);
                        }
                     }
                  
                  }
                  else if(preservedShape instanceof SKCircleShape)
                  {
                     SKCircleShape sh1 = (SKCircleShape)preservedShape;
                     float rad, rad1;
                     if(sh1.radius == -1) rad = 20;
                     else rad = (float)sh1.radius;
                     for (int i=0; i<con.ShapeList.size(); i++)
                     {SKBaseShape sh;
                        sh = con.ShapeList.get(i);
                        if (sh != preservedShape)
                           if(sh instanceof SKCircleShape)
                           {
                              if(((SKCircleShape)sh).radius == -1) rad1 = 20;
                              else rad1 = (float)((SKCircleShape)sh).radius;
                              int centerx = ((SKCircleShape)sh).center.getShapeX();
                              int centery = ((SKCircleShape)sh).center.getShapeY();
                              double slope,c1,c2;
                              double[] input = new double[8];
                              double[] output = new double[8];
                              slope =((double) (sh1.center.getShapeY()-((SKCircleShape)sh).center.getShapeY())/(double)(sh1.center.getShapeX()-((SKCircleShape)sh).center.getShapeX()));
                              int x,y,tempx,tempy;
                              c1 = centery - (slope * centerx);
                              input[0] = centerx;
                              input[1] = centery;
                              input[2] = rad1;
                              input[3] = slope;
                              input[4] = c1;
                              input[5] = sh1.getShapeX();
                              input[6] = sh1.getShapeY();
                              circleintersection(input,output);
                              x=(int)output[0];
                              y=(int)output[1];
                              input[0] = sh1.center.getShapeX();
                              input[1] = sh1.center.getShapeY();
                              input[2] = rad;
                              input[5] = sh.getShapeX();
                              input[6] = sh.getShapeY();
                              circleintersection(input,output);
                              x = (int)output[0] - x;
                              y = (int)output[1] - y;
                              tempx = (int)output[0];
                              tempy = (int)output[1];
                              sh.doMove(x,y,false);
                              ResolveSimpleConstraints(sh,cons);
                           }
                           else{
                              int centerx = ((SKCircleShape)sh1).center.getShapeX();
                              int centery = ((SKCircleShape)sh1).center.getShapeY();
                              double slope,pslope,c1,c2;
                              double[] input = new double[8];
                              double[] output = new double[8];
                              double[] array = new double[8];
                              slope =((double) (((SKLineShape)sh).pt2.getShapeY()-((SKLineShape)sh).pt1.getShapeY())/(double)(((SKLineShape)sh).pt2.getShapeX()-((SKLineShape)sh).pt1.getShapeX()));
                              pslope = -1/slope;
                              c1 = (((SKLineShape)sh).pt2.getShapeY() - (slope*((SKLineShape)sh).pt2.getShapeX()));
                              c2 = centery - (pslope * centerx);
                              int x,y;
                              if(((SKLineShape)sh).pt2.getShapeY()==((SKLineShape)sh).pt1.getShapeY())
                                 if (((SKLineShape)sh).pt2.getShapeY()>centery)
                                    sh.doMove(0,(int)(-((SKLineShape)sh).pt2.getShapeY()+centery+rad),false);
                                 else 
                                    sh.doMove(0,(int)(((SKLineShape)sh).pt2.getShapeY()+centery-rad),false);
                              else if(((SKLineShape)sh).pt2.getShapeX()==((SKLineShape)sh).pt1.getShapeX())
                                 if (((SKLineShape)sh).pt2.getShapeX()>centerx)
                                    sh.doMove(0,(int)(-((SKLineShape)sh).pt2.getShapeX()+centery+rad),false);
                                 else 
                                    sh.doMove(0,(int)(((SKLineShape)sh).pt2.getShapeX()+centery-rad),false);
                              
                              else
                              {
                                 c1 = (((SKLineShape)sh).pt2.getShapeY() - (slope*((SKLineShape)sh).pt2.getShapeX()));
                                 c2 = centery - (pslope * centerx);
                                 lineintersection(array,slope,pslope,c1,c2);
                                 x=(int)array[0];
                                 y=(int)array[1];
                                 input[0] = centerx;
                                 input[1] = centery;
                                 input[2] = rad;
                                 input[3] = pslope;
                                 input[4] = c2;
                                 input[5] = sh.getShapeX();
                                 input[6] = sh.getShapeY();
                                 circleintersection(input,output);
                                 x = (int)output[0] - x;
                                 y = (int)output[1] - y;
                                 sh.doMove(x,y,false);
                              }
                              ResolveSimpleConstraints(sh,cons);
                           
                           }
                     }
                  
                  }
               }
               break;
         
            default: res = false;
         }
      
         return res;
      }
   
   
      public static float distance(int x1, int y1, int x2, int y2)
      
      {
         return (float)java.lang.Math.sqrt(java.lang.Math.pow((double)(x1-x2),2)+java.lang.Math.pow((double)(y1-y2),2));
      }
   
   
      public static boolean lineintersection( double array[], double slope1, double slope2, double c1, double c2 )
      
      {
         System.out.println(" line "+slope1+" "+slope2+" "+c1+" "+c2);
         array[0] = (c2 - c1) / (slope1 - slope2);
         array[1] = ( (slope1*c2) - (slope2*c1) ) / ( slope1 - slope2);
         if((slope1==0) || (slope2==0))
            return false;
         else
            return true;
      }
   
   
   
      public static void circleintersection( double input[], double output[])
      
      
      {
         int x=(int)input[0], y=(int)input[1], px=(int)input[5], py=(int)input[6];
         double m=input[3], c=input[4],a1,a2,x1,x2,y1,y2,r;
         float dist1,dist2;
         if(input[2]==-1)
            r = 20;
         else r = input[2];
         a1 = ((2*x) - (2*m*c) + (2*m*y)) / ((m*m) +1);
         a2 = ((x*x) + (c*c) -(2*y*c) + (y*y) - (r*r)) / ((m*m) +1);
         x1 = (a1 + java.lang.Math.sqrt((a1*a1) - (4*a2)) )/2;
         x2 = (a1 - java.lang.Math.sqrt((a1*a1) - (4*a2)) )/2;
         y1 = (m*x1) +c;
         y2 = (m*x2) +c;
         dist1 = distance((int)x1,(int)y1,px,py);
         dist2 = distance((int)x2,(int)y2,px,py);
         if(dist1<dist2)
         { 
            output[0] = x1;
            output[1] = y1;
         }
         else
         { 
            output[0] = x2;
            output[1] = y2;
         }
      }
   
   
      public static SKBaseShape getOtherShape(SKBaseConstraint con, SKBaseShape sh)
      
      {
         if( (SKBaseShape) con.ShapeList.get(0) == sh)
            return (SKBaseShape) con.ShapeList.get(1);
         else 
            return (SKBaseShape) con.ShapeList.get(0);
      }
   
   
      public  static void checkTangency(SKBaseShape shape)
      
      {
         SKBaseConstraint con;
         for(int i=0; i<shape.ConstraintList.size(); i++)
         {con = shape.ConstraintList.get(i);
            if(con instanceof SKTangentConstraint)
            {SKCircleShape sh1;
               SKBaseShape sh2;
               if( con.ShapeList.get(0) instanceof SKCircleShape)
               {sh1= (SKCircleShape)con.ShapeList.get(0);
                  sh2 = con.ShapeList.get(1);}
               else 
               {sh1= (SKCircleShape)con.ShapeList.get(1);
                  sh2 = con.ShapeList.get(0);}
               double r1,r2;
               if(sh1.radius == -1) r1=20;
               else r1 = sh1.radius*40*sh1.scale;
               if(sh2 instanceof SKCircleShape) 	
               {
                  double d = distance(sh1.getShapeX(), sh1.getShapeY(), sh2.getShapeX(), sh2.getShapeY());
                  if(((SKCircleShape)sh2).radius == -1) r2=20;
                  else r2 = ((SKCircleShape)sh2).radius*40*((SKCircleShape)sh2).scale;
                  if((int)(r1+r2+20) != (int)d)
                     increase(con);
               }
               else
               {double d= ((SKLineShape)sh2).lineShape.ptLineDist(sh1.getShapeX(), sh1.getShapeY());
                  System.out.println(r1+" "+d);
                  if((int)(r1+10) != (int)d)
                     increase(con);
               }
            }
         }
      }
   
   
      public static void increase(SKBaseConstraint c)
      
      {
         SKCircleShape sh=null;
         SKTangentConstraint con = (SKTangentConstraint)c;
         double dist;
         for(int i=0; i<con.ShapeList.size(); i++)
            if(con.ShapeList.get(i) instanceof SKCircleShape)
               sh = (SKCircleShape)con.ShapeList.get(i);
         SKBaseShape sh1 = getOtherShape(con, sh);
         if(sh1 instanceof SKLineShape)
            dist = ((SKLineShape)sh1).lineShape.ptLineDist((double)sh.getShapeX(),(double)sh.getShapeY());
         else
            dist = (double)distance(sh.getShapeX(), sh.getShapeY(), sh1.getShapeX(), sh1.getShapeY());
         System.out.println(dist);
         sh.increaseRadius(dist);
      }
   /*********************************************************Simple Solver version2.0*********************************************************/
   
      public static void ResolveSimpleConstraints(SKBaseShape shape)
      
      {
         cons = new SKConstraintArray(5);
         shapes = new SKShapeArray(5);
         count=0;
      
         System.out.println("move========================");
         collectConstraintsByShape(shape);
         shapes.bubblesort(); //based on # of parallel and perpendicular
         assignOrientConstraint();
         solveDist();
         solveIncid();
         double max_move=1.0;
         while(max_move>0.05&&count<50)
         {
            max_move=solveOrient();
            System.out.println("max_move="+max_move);
            count++;
         }
      
      
         for (int i=0; i<shapes.size(); i++)
            System.out.println("shape="+shapes.get(i)
                              +"  #="+shapes.get(i).orientCons
                              +"  for="+shapes.get(i).toServeConstraint);
      
         for (int i=0; i<cons.size(); i++)
            System.out.println("con="+cons.get(i)+"  by="+cons.get(i).beServedBy);
      
      }
      public static void ResolveSimpleConstraints(SKBaseShape shape, SKConstraintArray cons)
      {
         SKBaseConstraint con;
         for (int i=0; i<shape.ConstraintList.size(); i++)
         {
            con = shape.ConstraintList.get(i);
            if (cons.indexOf(con) == -1)
            {
               DoSimpleSolver(con,shape,cons);
            }
         }
         //checkTangency(shape);
      }
   //Solves 1 constraint (preserving the position of 1 shape)
      public static boolean DoSimpleSolver(SKBaseConstraint con)
      
      {
         cons = new SKConstraintArray(5);
         shapes = new SKShapeArray(5);
         count=0;
      
         System.out.println("add---------------------");
         collectConstraintsBycon(con);
         shapes.bubblesort(); //based on # of parallel and perpendicula
         assignOrientConstraint();
         solveDist();
         solveIncid();
         double max_move=1.0;
         while(max_move>0.05&&count<50)
         {
            max_move=solveOrient();
            System.out.println("max_move="+max_move);
            count++;
         }
      
         for (int i=0; i<shapes.size(); i++)
            System.out.println("shape="+shapes.get(i)
                              +"  #="+shapes.get(i).orientCons
                              +"  for="+shapes.get(i).toServeConstraint);
      
         for (int i=0; i<cons.size(); i++)
            System.out.println("con="+cons.get(i)+"  by="+cons.get(i).beServedBy);
      
         return false;
      }
   
      private static void collectConstraintsByShape(SKBaseShape shape)
      
      {
         if(shapes.indexOf(shape) == -1)
         {
            shape.toServeConstraint=0;
            shapes.add(shape);
            int orientContraints=0;
            for (int i=0; i<shape.ConstraintList.size(); i++)
            {
               SKBaseConstraint con = shape.ConstraintList.get(i);
               if(con.typeID==2||(con.typeID==3||con.typeID==4))
                  orientContraints++;
               collectConstraintsBycon(con);
            }
            shape.orientCons=orientContraints;
         }
      }
   
   
      private static void collectConstraintsBycon(SKBaseConstraint con)
      
      {
         if(cons.indexOf(con) == -1)
         {
            con.beServedBy=0;
            cons.add(con);
            for (int i=0; i<con.ShapeList.size(); i++)
            {
               SKBaseShape sh = con.ShapeList.get(i);
               collectConstraintsByShape(sh);
            }
         }
      }
   
   
      private static void assignOrientConstraint()
      
      {
         for(int i=0; i<shapes.size(); i++)
         {
            SKBaseShape sh1=shapes.get(i);
            for(int j=0; j<sh1.ConstraintList.size(); j++)
            {
               SKBaseConstraint con=sh1.ConstraintList.get(j);
               if((con.typeID==2||con.typeID==3||con.typeID==4)&&(con.beServedBy==0))
               {
                  SKBaseShape sh2=con.ShapeList.get(0);
                  if(sh1==sh2)
                     sh2=con.ShapeList.get(1);
                  if(sh2.toServeConstraint==0)
                  {
                     sh2.toServeConstraint=con.ID;
                     con.beServedBy=sh2.ID;
                  }
                  else if(sh1.toServeConstraint==0)
                  {
                     sh1.toServeConstraint=con.ID;
                     con.beServedBy=sh1.ID;
                  }
               }
            }
         }
      }
   
   
      private static void solveIncid()
      
      {
         for (int i=0; i<cons.size(); i++)
         {
            SKBaseConstraint con=cons.get(i);
            if(con.typeID==1) //Incidence
            {
               SKBaseShape preservedShape = con.ShapeList.get(0);
               SKBaseShape sh = con.ShapeList.get(1);
               if(sh.ID<preservedShape.ID)
               {
                  sh = con.ShapeList.get(0);
                  preservedShape = con.ShapeList.get(1);
               }
            
               Point pt1,pt2;
            
               int g = con.getConInfo(sh,0);
               pt1 = sh.getConstraintPoint(con,g);
               pt2 = preservedShape.getConstraintPoint(con,con.getConInfo(preservedShape,0));
               if (!pt1.equals( pt2 ))
               {
                  sh.setConstraintPoint(con,g,pt2);
               }
            }
         }
      }
   
      private static void setDist(double dist, int fixX, int fixY, int movX, int movY, SKNormalShape movePt)
      
      {
         double m=Double.MAX_VALUE;
	 if(fixX!=movX)m=(fixY-movY)/(fixX-movX); 
         double deltaX=(dist*dist*1600)/(1+m*m);
         if(fixX>movX)
            deltaX=-java.lang.Math.sqrt(deltaX);
         else deltaX=java.lang.Math.sqrt(deltaX);      
         float tempX;
         float tempY;
         tempX=(float) fixX+((float) deltaX);
         tempY=(float) fixY+((float) m)*((float) deltaX); 
         movePt.doMove((int) (tempX-movX), (int) (tempY-movY), false);         
      }
   
      private static void solveDist()  
      
      {
         for (int i=0; i<cons.size(); i++)
         {
            SKBaseConstraint con=cons.get(i);
            if(con.typeID==0) //Distance
            {
               System.out.println("DistSolver Running");
               SKDistanceConstraint theDist =(SKDistanceConstraint) con;
               SKBaseShape preservedShape = con.ShapeList.get(0);
               SKBaseShape sh = con.ShapeList.get(1);
               if(sh.ID<preservedShape.ID)
               {  
                  sh = con.ShapeList.get(0);
                  preservedShape = con.ShapeList.get(1);
               }
            
               int shType=sh.getShapeTypeID();
               int preservedType=preservedShape.getShapeTypeID();
               SKNormalShape preservedPt1, preservedPt2, shPt1, shPt2;               
            
               switch(shType*10+preservedType)
               {
                  case 40:
                  case 0: preservedPt1=(SKNormalShape) preservedShape.getSubShape(0);
                     shPt1=(SKNormalShape) sh.getSubShape(0);
                     setDist(theDist.distance, preservedPt1.getX(), preservedPt1.getY(), shPt1.getX(), shPt1.getY(), shPt1);
                     break;
                  case 41:
                  case 42:
                  case 43:
                  case 1: 
                  case 2:
                  case 3: shPt1=(SKNormalShape) sh.getSubShape(0);
                     int g=con.getConInfo(preservedShape,0);
                     System.out.println("g="+g);
                     if(g!=0)
                     {
                        Point pt;
                        pt = ((SKLineShape) preservedShape).getConstraintPoint(con,g);
                        System.out.println("X="+pt.x+", Y="+pt.y);
                        setDist(theDist.distance, pt.x, pt.y, shPt1.getX(), shPt1.getY(), shPt1);
                     }
                     else
                     {
                        preservedPt1=(SKNormalShape) preservedShape.getSubShape(1);
                        preservedPt2=(SKNormalShape) preservedShape.getSubShape(2);
                        double m=(preservedPt1.getY()-preservedPt2.getY());
                        m=m/(preservedPt1.getX()-preservedPt2.getX());
                        m=-1/m;
                        double deltaX=(theDist.distance*theDist.distance*1600)/(1+m*m);
                        deltaX=java.lang.Math.sqrt(deltaX);  
                        float tempX;
                        float tempY;
                        Point pt;
                        pt = ((SKLineShape) preservedShape).getConstraintPoint(con,g);
                        if(shPt1.getX()<pt.x) deltaX=-deltaX;
                        tempX=(float) pt.x+((float) deltaX); 
                        tempY=(float) pt.y+((float) m)*((float) deltaX);
                        shPt1.doMove((int) (tempX-shPt1.getX()), (int) (tempY-shPt1.getY()), false);
                     }
                     break;
                  case 4: shPt1=(SKNormalShape) sh.getSubShape(0);
                     g=con.getConInfo(preservedShape,0);
                     Point pt = ((SKCircleShape) preservedShape).getConstraintPoint(con,g);
                     setDist(theDist.distance, pt.x, pt.y, shPt1.getX(), shPt1.getY(), shPt1);
                     break;
                  case 14:
                  case 24:
                  case 34:
                  case 10: 
                  case 20:
                  case 30: preservedPt1=(SKNormalShape) preservedShape.getSubShape(0);
                     shPt1=(SKNormalShape) sh.getSubShape(1);
                     shPt2=(SKNormalShape) sh.getSubShape(2);
                     g=con.getConInfo(sh,0);
                     System.out.println("g="+g);
                     if(g!=0)
                     {  
                        if(g==1)
                           setDist(theDist.distance, preservedPt1.getX(), preservedPt1.getY(), shPt1.getX(), shPt2.getY(), shPt1);
                        else setDist(theDist.distance, preservedPt1.getX(), preservedPt1.getY(), shPt2.getX(), shPt2.getY(), shPt2);
                     }
                     else
                     {
                        shPt1=(SKNormalShape) sh.getSubShape(1);
                        shPt2=(SKNormalShape) sh.getSubShape(2);
                        double m=(shPt1.getY()-shPt2.getY());
                        m=m/(shPt1.getX()-shPt2.getX());
                        m=-1/m;
                        double deltaX=(theDist.distance*theDist.distance*1600)/(1+m*m);
                        deltaX=java.lang.Math.sqrt(deltaX);
                        float tempX;
                        float tempY;
                        pt = ((SKLineShape) sh).getConstraintPoint(con,g);
                        if(preservedPt1.getX()<pt.x) deltaX=-deltaX;
                        tempX=(float) pt.x+((float) deltaX);
                        tempY=(float) pt.y+((float) m)*((float) deltaX);
                        preservedPt1.doMove((int) (tempX-preservedPt1.getX()), (int) (tempY-preservedPt1.getY()),false);
                     }
                     break;
               }
            }
         }
      }
   
   
      private static double solveOrient()
      
      {
         double max_move=0.0;
         for(int i=0; i<shapes.size(); i++)
         {
            SKBaseShape sh = shapes.get(i);
            for(int j=0; j<sh.ConstraintList.size(); j++)
            {
               SKBaseConstraint con=sh.ConstraintList.get(j);
               double move=solveOneOrient(con);
               if(max_move<move)
                  max_move=move;
            }
         }
         return max_move;
      }
   
   
      private static double solveOneOrient(SKBaseConstraint con)
      
      {
         double move=0.0;
         if(con.typeID==2||con.typeID==3||con.typeID==4) // 2-Perpendicular, 3-parallel
         {
            SKBaseShape preservedShape = con.ShapeList.get(0);
            SKBaseShape sh = con.ShapeList.get(1);
            if(preservedShape.ID==con.beServedBy)
            {
               sh = con.ShapeList.get(0);
               preservedShape = con.ShapeList.get(1);
            }
         
            SKPointShape leftPoint, rightPoint, fixPt, movPt;
            leftPoint = ((SKLineShape)preservedShape).getLeftPoint();
            rightPoint = ((SKLineShape)preservedShape).getOtherPoint(leftPoint);
            double
            p01x = leftPoint.getX(),
            p01y = leftPoint.getY(),
            p02x = rightPoint.getX(),
            p02y = rightPoint.getY(),
            p_dltx,
            p_dlty;
         
            double p_dist;
         
            if(con.typeID==2) //2-Perpendicular
            {        
               p_dltx=p02y-p01y;
               p_dlty=p01x-p02x;
               p_dist = java.lang.Math.sqrt(p_dltx*p_dltx+p_dlty*p_dlty);
            }
            else if(con.typeID==3)  //3-parallel
            {
               p_dltx=p02x-p01x;
               p_dlty=p02y-p01y;
               p_dist = java.lang.Math.sqrt(p_dltx*p_dltx+p_dlty*p_dlty);
            }
            else
            {
               SKAngleConstraint tempCon = (SKAngleConstraint) con;
               p_dltx=p02x-p01x;
               p_dlty=p02y-p01y;
               p_dist = java.lang.Math.sqrt(p_dltx*p_dltx+p_dlty*p_dlty);
            
               double tanAngle;
               tanAngle=java.lang.Math.atan(p_dlty/p_dltx);
               System.out.println("Tangent: "+tanAngle);
               tanAngle+=tempCon.angle/180*java.lang.Math.PI;
               p_dltx=p_dist*java.lang.Math.cos(tanAngle);
               p_dlty=p_dist*java.lang.Math.sin(tanAngle);
            }
         
            double fixX, fixY, movX, movY, newx1, newy1, newx2, newy2,
            newx, newy, s_dltx, s_dlty, s_dist;
         
            String fixPoint0 = new String("right");
            movPt = ((SKLineShape)sh).getLeftPoint();
            fixPt = ((SKLineShape)sh).getOtherPoint(movPt);
         
            if((movPt.getX()==p01x&&movPt.getY()==p01y)||
                  (movPt.getX()==p02x&&movPt.getY()==p02y) )
            {
               fixPoint0 = new String("left");
               fixPt = ((SKLineShape)sh).getLeftPoint();
               movPt = ((SKLineShape)sh).getOtherPoint(fixPt);
            }
         
            if(count>10)
            {
               SKPointShape temp=fixPt;
               fixPt=movPt;
               movPt=temp;
               int minNeighborID=sh.ID;
            
               String fixPoint1 = new String("left");
               for (int i=0; i<sh.ConstraintList.size(); i++)
               {
                  SKBaseConstraint conIncid = sh.ConstraintList.get(i);
                  SKBaseShape otherShape = conIncid.ShapeList.get(0);
                  if(otherShape.ID==sh.ID)
                     otherShape = conIncid.ShapeList.get(1);
               
                  if(conIncid.typeID==1&&minNeighborID>otherShape.ID)
                  {
                     minNeighborID=otherShape.ID;
                     int g = conIncid.getConInfo(sh,0);
                     Point pt1 = sh.getConstraintPoint(con,g);
                     Point pt2 = sh.notConstraintPoint(con,g);
                  
                     if(pt1.getX()<pt2.getX())
                        fixPoint1 = new String("left");
                     else if (pt1.getX()==pt2.getX()&&pt1.getY()<pt2.getY())
                        fixPoint1 = new String("left");
                     else
                        fixPoint1 = new String("right");
                  }
               }
            
               if(count<20)
               {
                  if(fixPoint1.equals("right"))
                  {
                     movPt = ((SKLineShape)sh).getLeftPoint();
                     fixPt = ((SKLineShape)sh).getOtherPoint(movPt);
                  }
                  else
                  {
                     fixPt = ((SKLineShape)sh).getLeftPoint();
                     movPt = ((SKLineShape)sh).getOtherPoint(fixPt);
                  }
               }
            }
         
            fixX=fixPt.getX();
            fixY=fixPt.getY();
         //System.out.println("real fix="  +fixX +", "+ fixY);
            movX=movPt.getX();
            movY=movPt.getY();
            s_dltx=movX-fixX;
            s_dlty=movY-fixY;
            s_dist = java.lang.Math.sqrt(s_dltx*s_dltx+s_dlty*s_dlty);
         
            newx1= fixX + s_dist*p_dltx/p_dist;
            newy1= fixY + s_dist*p_dlty/p_dist;
            newx2= fixX - s_dist*p_dltx/p_dist;
            newy2= fixY - s_dist*p_dlty/p_dist;
         
            if(   ((newx1-movX)*(newx1-movX)+(newy1-movY)*(newy1-movY))
               < ((newx2-movX)*(newx2-movX)+(newy2-movY)*(newy2-movY)) )
            {
               newx = newx1;
               newy = newy1;
            }
            else
            {
               newx = newx2;
               newy = newy2;
            }
         
         
            double move_dist=java.lang.Math.sqrt(
                                 (newx - movX)*(newx - movX)+(newy - movY)*(newy - movY) );
            move=move_dist/s_dist;
         
            if( (move_dist/s_dist) <0.000001)
               System.out.println("no rotating");
            else
            {
               movPt.doMove((int) (newx - movX), (int) (newy - movY), false);
               sovleIncidRecurByShape(sh);
            }
         }
         return move;
      }
   
   
      private static void sovleIncidRecurByShape(SKBaseShape shape)
      
      {
         SKBaseConstraint con;
         for (int i=0; i<shape.ConstraintList.size(); i++)
         {
            con = shape.ConstraintList.get(i);
            sovleIncidRecurByConst(con, shape);
         }
      }
   
   
      private static void sovleIncidRecurByConst(SKBaseConstraint con, SKBaseShape preservedShape)
      
      {
         if(con.typeID==1)
         {
            int i,g;
            Point pt1,pt2;
         
            SKBaseShape sh=con.ShapeList.get(0);
            if(sh==preservedShape)
               sh=con.ShapeList.get(1);
         
            g = con.getConInfo(sh,0);
            pt1 = sh.getConstraintPoint(con,g);
            pt2 = preservedShape.getConstraintPoint(con,con.getConInfo(preservedShape,0));
            if (!pt1.equals( pt2 ))
            {
               sh.setConstraintPoint(con,g,pt2);
               sovleIncidRecurByShape(sh);
            }
         }
      }
   
   }
