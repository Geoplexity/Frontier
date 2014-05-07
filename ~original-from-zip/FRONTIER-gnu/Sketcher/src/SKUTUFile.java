/**
 * Title:        SKUTUFile
 * Description:  Methods to create file for UTU
 */
/*Copyright (C) June 22, 2001 Meera Sitharam

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
http://www.gnu.org/copyleft/gpl.html

You should have received a copy of the GNU General Public License
in the documentation index in the documentation
folder of the FRONTIER-gnu directory; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

   package sketcher;

   import java.awt.Container;
   import java.awt.Point;
   import java.io.*;
   import java.awt.geom.*;
   import utuJava;

   public class SKUTUFile
   {
      static int dataInt[];
      static int actCluster=0, indexInt=0,indexDbl=0;
      static double dataDouble[];
      static SKGTNArray dag;
   
      public static boolean waitForUpdate(int[] IntArray, double[] DblArray, SKShapeArray shapes,SKMainFrame frame, int width, int height, SKGTNArray dagMain )
      {  
         SKBaseShape sh;
         TreeDialog clusters=null;
         TreeFrame actClusters = null;
         utuJava utuDriver = new utuJava();
         SKGroupTreeNode root = null, enode = null;
         SKGTNArray roots = new SKGTNArray(10);
         SKGTNArray copyDag = new SKGTNArray(10);
         SKShapeArray allShapes = new SKShapeArray(10);
         shapes.copyArrayTo(allShapes);
         int wid = width, hgt = height, w = width >> 1, h = height >> 1,  maxX, minX, maxY, minY ,numSolutions, delta=0;
         int g,x,y,x3=0,y3=0,x2=0,y2=0,count=shapes.size(),number;
         double num1=0,num2=0;
         boolean done=false;
         maxX = -999;
         minX = 999;
         maxY = -999;
         minY = 999;
         dataInt = IntArray;
         dataDouble = DblArray;
         indexInt = dataInt[0];
         indexDbl = (int)dataDouble[0];
         numSolutions = dataInt[indexInt++];
      
         if(numSolutions == -2)
         {
            dag = new SKGTNArray(100);
            int numRoots= dataInt[indexInt++];
            int ClusterID = dataInt[indexInt++];
            while(ClusterID != -1)
            {
               boolean isPresent=false;
               for(int n=0; n<dag.size(); n++)
                  isPresent= isPresent || (dag.get(n).ID==ClusterID);
               if(!isPresent)
               {
                  SKGroupTreeNode node = new SKGroupTreeNode(null,ClusterID,"Group"+ClusterID);
                  dag.add(node);
                  enode = (SKGroupTreeNode)node;
               }
               else                            
                  for(int n=0; n<dag.size(); n++) 
                     if(dag.get(n).ID==ClusterID)
                        enode = (SKGroupTreeNode)dag.get(n);
            
               int ssize= dataInt[indexInt++];
            
               SKShapeArray Shapes = new SKShapeArray(ssize);
            
               for(int m=0; m<ssize; m++)
               {
                  int SID = dataInt[indexInt++];
                  SKBaseShape shape = (SKBaseShape)frame.allshapes.findByID(SID);
                  Shapes.add(shape);
               }
            
               Shapes.copyArrayTo(enode.shapes);
               int numChildren = dataInt[indexInt++];
            
               for(int m=0; m<numChildren; m++)
               {
                  ClusterID = dataInt[indexInt++];
                  isPresent=false;
                  for(int n=0; n<dag.size(); n++)
                     isPresent= isPresent || (dag.get(n).ID==ClusterID);
                  if(!isPresent)
                  {
                     SKGroupTreeNode node1 = new SKGroupTreeNode(enode,ClusterID,"Cluster"+ClusterID);
                     enode.children.add(node1);
                     dag.add(node1);
                  }
                  else 
                     for(int n=0; n<dag.size(); n++) 
                        if(dag.get(n).ID==ClusterID)
                           enode.children.add(dag.get(n));
               }
            
               ClusterID = dataInt[indexInt++];
            }
         
            for(int l=0; l<dag.size(); l++)
               for(int m=0; m<dag.get(l).children.size(); m++)
                  for( int n =0 ; n<dag.size(); n++)
                     if(dag.get(n)==dag.get(l).children.get(m))
                        dag.get(n).parents.add(dag.get(l));
         
            for(int l=0; l<dag.size(); l++)
               if (dag.get(l).parents.size()==0)
                  roots.add(dag.get(l));
            dag.copyArrayTo(dagMain);
            int[] fin = new int[dag.size()];
            for(int i=0; i<fin.length; i++)
               fin[i]=1;
            clusters = new TreeDialog(frame,dag,actCluster,fin,true);
            //clusters = new TreeDialog(frame,dag,actCluster,true);
            clusters.setLocation(691,54);
            clusters.show();
            return clusters.approved;
               //dataInt[0] = 1;
               //utuDriver.utuC(dataInt, dataDouble);
         }
         
         else if(numSolutions == 1)
         {   	//Solved, parse and update shapes
            boolean first=true, noCommon;
            int i;
            frame.update = false;
            frame.mniOptions.setEnabled(false);
            frame.mniTest.setEnabled(false);   
            frame.mniAutoSolve.setEnabled(false);
            dagMain.copyArrayTo(dag);
            int noRoots = dataInt[indexInt++];
            for( int a =0; a<noRoots;a++)
            {int rootID = dataInt[indexInt++];
            number = dataInt[indexInt++];
            /*SKGTNArray roots1= new SKGTNArray(2);
            SKShapeArray commons = new SKShapeArray(2);
            SKShapeArray doneCommon = new SKShapeArray(2);
            for(int l=0; l<dag.size(); l++)
               if (dag.get(l).parents.size()==0)
                  roots.add(dag.get(l));
         
            for(int l=0; l<roots.size(); l++)
               for(int m=l+1; m<roots.size(); m++)
               {noCommon = true;
                  for(int n=0; n<roots.get(l).shapes.size(); n++)
                  {
                     noCommon = noCommon && (roots.get(m).shapes.indexOf(roots.get(l).shapes.get(n))==-1);
                     if(!(roots.get(m).shapes.indexOf(roots.get(l).shapes.get(n))==-1))
                        //for(int o=0; o<roots.get(l).shapes.get(n).getNumSubShapes(); o++)
                        commons.add(roots.get(l).shapes.get(n));
                  }
               }*/
            for(i=indexInt; i<indexInt+6; i++)
            System.out.println("integer "+dataInt[i]);
            for(i=indexDbl; i<indexDbl+12; i++)
            System.out.println("double "+dataDouble[i]);
            for(i=1;i<=number;i++)
            {
               g = dataInt[indexInt++];
               sh = shapes.findByID(g);
               num1 = dataDouble[indexDbl++];
               num2 = dataDouble[indexDbl++];
            
               x = (int)java.lang.Math.round( num1*40 ) + w;
               y = (int)java.lang.Math.round( num2*40 ) + h;
            
               if (x > maxX)  maxX = x;
               if (x < minX)  minX = x;
               if (y > maxY)  maxY = y;
               if (y < minY)  minY = y;
            
               for(int j=0; j<roots.size();j++)
               {
                  if( roots1.indexOf(roots.get(j))==-1)
                     if( (roots.get(j).shapes.indexOf(sh)!=-1))
                     {
                        for(int k=0; k<roots.get(j).shapes.size(); k++)
                        //for(int m=0; m<roots.get(j).shapes.get(k).getNumSubShapes(); m++)
                           if((commons.indexOf(roots.get(j).shapes.get(k))!=-1) && (sh.ID!=roots.get(j).shapes.get(k).ID))
                           {int d=indexDbl;
                              for(int l=indexInt; l<indexInt+roots.get(j).shapes.size(); l++)
                              {
                                 if(dataInt[l]==roots.get(j).shapes.get(k).ID)
                                 {
                                    x3=(int)dataDouble[d];
                                    y3=(int)dataDouble[d+1];
                                 }
                                 else 
                                 {
                                    SKBaseShape temp = frame.allshapes.findByID(dataInt[l]);
                                    if(temp instanceof SKPointShape) d=d+2;
                                    else if(temp instanceof SKLineShape) d=d+4;
                                    else if(temp instanceof SKCircleShape) d=d+3;
                                 }
                              }
                              x2=(int)roots.get(j).shapes.get(k).getShapeX();
                              y2=(int)roots.get(j).shapes.get(k).getShapeY();
                           }
                           else
                           {
                              first = true;
                              roots1.add(roots.get(j));
                           }
                     } 
               }
            
               if (sh instanceof SKPointShape)
               {
                  if(first)
                  {
                     x3=x;
                     y3=y;
                     x2=(int)sh.getShapeX();
                     y2=(int)sh.getShapeY();
                     first=false;
                  }  
                  else 
                  {
                     x=x2-(x3-x);
                     y=y2-(y3-y);
                     sh.setShapePoint(new Point(x,y));
                  }
               }
               else if (sh instanceof SKLineShape)
               {
                  if(first)
                  {x3=x;
                     y3=y;
                     x2=(int)((SKLineShape)sh).pt1.getShapeX();
                     y2=(int)((SKLineShape)sh).pt1.getShapeY();
                     first=false;
                  }
                  else 
                  {
                     x=x2-(x3-x);
                     y=y2-(y3-y);
                     ((SKLineShape)sh).pt1.setShapePoint(new Point(x,y));
                  }
                  num1 = dataDouble[indexDbl++];
                  num2 = dataDouble[indexDbl++];
                  x = (int)java.lang.Math.round( num1*40 ) + w;
                  y = (int)java.lang.Math.round( num2*40 ) + h;
                  x=x2-(x3-x);
                  y=y2-(y3-y);
               
                  ((SKLineShape)sh).pt2.setShapePoint(new Point(x,y));
                        //int scale = Integer.parseInt( p.readLine() );
                        /*if (x > maxX)  maxX = x;
                        if (x < minX)  minX = x;
                        if (y > maxY)  maxY = y;
                        if (y < minY)  minY = y;*/
               }
               else if (sh instanceof SKCircleShape)
               {
                  if(first)
                  {
                     x3=x;
                     y3=y;
                     x2=(int)((SKCircleShape)sh).center.getShapeX();
                     y2=(int)((SKCircleShape)sh).center.getShapeY();
                     first=false;
                  }
                  else 
                  {
                     x=x2-(x3-x);
                     y=y2-(y3-y);
                     ((SKCircleShape)sh).center.setShapePoint(new Point(x,y));
                     ((SKCircleShape)sh).radius = dataDouble[indexDbl++];
                  }
               }
            }
            }
            //Update sketch so it fits on screen
            System.out.println("w "+width+" min x "+minX+" max x"+maxX+" min y "+minY+" max y "+maxY);
            if (minX < 0)
               frame.doTranslate(-minX,0);
            else if (maxX > width)
               frame.doTranslate(width-maxX,0);

            if (minY < 0)
               frame.doTranslate(-minY,0);
            else if (maxY > height)
               frame.doTranslate(height-maxY,0);

            delta = maxX-minX;
            if ((maxY-minY) > delta)
            {
               delta = maxY-minY + 20;
               if (delta > height)
               {
                  frame.doScale(height/delta);
                  delta = height/delta;
                  maxX *= delta;
                  minX *= delta;
                  maxY *= delta;
                  minY *= delta;
               }
               if(maxY-minY <250)
               {  System.out.println("w "+width+" min x "+minX+" max x"+maxX+" min y "+minY+" max y "+maxY);
                  frame.doScale(frame.lastScale*2);
                  maxX = (int)(maxX*2);
                  minX = (int)(minX*2);
                  maxY = (int)(maxY*2);
                  minY = (int)(minY*2);
                  System.out.println("done scale");
               }
            }
            else
            {
               delta += 20;
               if (delta > width)
               {
                  frame.doScale(width/delta);
                  delta = height/delta;
                  maxX *= delta;
                  minX *= delta;
                  maxY *= delta;
                  minY *= delta;
               }
               if(maxX-minX <250)
               {  System.out.println("w "+width+" min x "+minX+" max x"+maxX+" min y "+minY+" max y "+maxY);
                  frame.doScale(frame.lastScale*2);
                  maxX = (int)(maxX*2);
                  minX = (int)(minX*2);
                  maxY = (int)(maxY*2);
                  minY = (int)(minY*2);
                  System.out.println("done scale");
               }
            }
         
            for(int l=0; l<roots.size(); l++)
               for(int m=l+1; m<roots.size(); m++)
               {noCommon = true;
                  int cnt=0;
                  SKBaseShape common=null;
               
                  for(int n=0; n<roots.get(l).shapes.size(); n++)
                  {
                     noCommon = noCommon && (roots.get(m).shapes.indexOf(roots.get(l).shapes.get(n))==-1);
                     if(!(roots.get(m).shapes.indexOf(roots.get(l).shapes.get(n))==-1))
                     {
                        common = roots.get(l).shapes.get(n);
                        cnt++;
                     }
                  }
                  if(!noCommon &&(cnt==1))
                  {
                     int min = 999;
                     SKBaseShape tip =null;
                     for( i=0; i<roots.get(m).shapes.size(); i++)
                     {
                        if(common.ID != roots.get(m).shapes.get(i).ID)
                        { 
                           SKBaseShape t = roots.get(m).shapes.get(i);
                           if(min>t.getX())
                           {
                              tip = t;
                              min = t.getX();
                           }
                        }
                     }
                     if(tip == null) tip = common;
                     //System.out.println("tip "+tip);
                     int p1x=common.getShapeX(), p1y=common.getShapeY(), p2x=tip.getShapeX(), p2y=tip.getShapeY(), px=0, py=0;
                     boolean outside=false;
                     double rad=0;
                     double pSlope , c1;
                     if(common.ID == tip.ID) 
                     {p1y=0;
                        p2y=300;}
                     if(p1x==p2x)
                        pSlope = Double.MAX_VALUE;
                     else pSlope = (p1y-p2y)/(p1x-p2x);
                     c1 = p1y - (pSlope*p1x);
                     p1x=0;
                     p1y=(int)((p1x*pSlope) + c1);
                     p2y=999;
                     p2x=(int)((p2y-c1)/pSlope);
                  
                     while(!outside && (rad<=java.lang.Math.PI*2))
                     {
                        outside = true;
                        for( i=0; i<roots.get(l).shapes.size(); i++)
                        {
                           px = roots.get(l).shapes.get(i).getShapeX();
                           py = roots.get(l).shapes.get(i).getShapeY();
                           outside = outside && (((py-p1y)*(p2x-p1x))-((px-p1x)*(p2y-p1y)) > 0);
                           //System.out.println(roots.get(l).shapes.get(i)+" "+outside);
                        }
                        if(!outside)
                        {
                           float r = distance(p1x,p1y,p2x,p2y);
                           rad = rad+(java.lang.Math.PI/36);
                           AffineTransform T = new AffineTransform();
                           T.setToRotation(rad,common.getShapeX(),common.getShapeY());
                        
                           float[] src,dst;
                        
                           src = new float[2];
                           dst = new float[2];
                           src[0]=p2x;
                           src[1]=p2y;
                           T.transform(src,0,dst,0,1);
                           p2x=(int)dst[0];
                           p2y=(int)dst[1];
                           //System.out.println("second "+p2x+" "+p2y+" "+rad);
                           for( i=0; i<roots.get(m).shapes.size(); i++)
                           {
                              /* for(int k=0; k<pnlShapes.getComponentCount(); k++)
                              for(int j=0; j<((SKBaseShape)pnlShapes.getComponent(k)).getNumSubShapes(); j++)
                              if(solvingShapes.get(i).ID == ((SKBaseShape)pnlShapes.getComponent(k)).getSubShape(j).ID)
                              {
                              px = ((SKBaseShape)pnlShapes.getComponent(k)).getSubShape(j).getShapeX();
                              py = ((SKBaseShape)pnlShapes.getComponent(k)).getSubShape(j).getShapeY();
                              }*/
                              px = roots.get(m).shapes.get(i).getShapeX();
                              py = roots.get(m).shapes.get(i).getShapeY();
                              src[0]=px;
                              src[1]=py;
                              T.transform(src,0,dst,0,1);
                              SKBaseShape shape;
                              for(int k=0; k<frame.panelShapeArea.getComponentCount(); k++)
                                 for(int j=0; j<((SKBaseShape)frame.panelShapeArea.getComponent(k)).getNumSubShapes(); j++)
                                    if(roots.get(m).shapes.get(i).ID == ((SKBaseShape)frame.panelShapeArea.getComponent(k)).getSubShape(j).ID) 
                                    {
                                       shape=((SKBaseShape)frame.panelShapeArea.getComponent(k)).getSubShape(j);
                                       shape.setShapePoint(new Point((int)dst[0],(int)dst[1]));
                                       //System.out.println(shape+" "+dst[0]+" "+dst[1]);
                                          //System.out.println("set "+sh+" "+px+" "+py);
                                    }
                           
                           }
                        }
                     }
                  }
               
                  done = true;
                  frame.update = false;
                  frame.panelPartial.removeAll();
               }
         
         
            return false;
         }
         else if(numSolutions == 0)
         {  
            dagMain.copyArrayTo(dag);
            for(int l=0; l<dag.size(); l++)
               if (dag.get(l).parents.size()==0)
                  roots.add(dag.get(l));
            int c = dataInt[0];  
            int numFiles = dataInt[indexInt++];
            int[] index= new int[2];
            int[] fin = new int[dag.size()];
            int elements = dataInt[indexInt++];
         
           // for(int i=c; i<c+elements ;i++)
               //System.out.println(dataInt[i]);
         
            for(int i=0; i<fin.length; i++)
               fin[i]=1;
         
            for(int i=0; i<elements; i++)
            {
               int ind =dataInt[indexInt++];
               ind=dag.indexOf(dag.findByID(ind));
               fin[ind] = dataInt[indexInt++];
            }
            for(int j=0; j<numFiles; j++)
            {  
               actCluster = dataInt[indexInt++]; 
               actClusters = new TreeFrame(frame,dag,actCluster,fin);
               actClusters.setLocation(691,54);
               actClusters.show();
               int numbifur = dataInt[indexInt++];                  
                  //Show the dialog
               SKBifurFrame dlg = new SKBifurFrame(frame,"Bifurcations",true);
               dlg.bifurcations = new SKBifurcationArray(5);
               index[0]=indexInt;
               index[1]=indexDbl;                      
               dlg.bifurcations.readFromStream(dataInt,dataDouble,index,numbifur,allShapes,frame,dlg.pnlShapes);
               dlg.setLocation(691,420);
               dlg.show();
               if (!dlg.approved )
                  return false;
               indexInt=index[0];
               indexDbl=index[1];
               boolean canFix=true;
               for(int a=0 ; a<roots.size(); a++)
                  for(int i=0; i<roots.get(a).children.size(); i++)
                     if(actCluster==roots.get(a).children.get(i).ID) canFix=false;
               for(int i= 0; i<dlg.pnlShapes.getComponentCount(); i++)
               {SKBaseShape sh1= (SKBaseShape)dlg.pnlShapes.getComponent(i);
                  if(canFix) frame.allshapes.findByID(sh1.ID).fixed=true;}
                  //return index of selected bifurcation
               dataInt[c++] = dlg.currBifurcation;
               //System.out.println("utufile "+dlg.currBifurcation);
               //dataInt[0] = 1;
               actClusters.hide();
            }
         
            return true;
              // utuDriver.utuC(dataInt, dataDouble);
         }
         return false;
         //}
      }
      public static void writeUTUFile(int[] IntArray, double[] DblArray, SKShapeArray shapes, SKGroupTreeNode groupRoot)
      {  
         dataInt = IntArray;
         dataDouble = DblArray;
         indexInt = 0;
         indexDbl = 0;
         dataInt[indexInt++]=0; //flag for UTU
         for(int j=0; j<dataInt.length; j++)
            dataInt[j] = 0;
         for(int j=0; j<dataDouble.length; j++)
            dataDouble[j]=0;
      	//Write shape list
         SKBaseShape sh,tmp;
         int i,g,h;
         for (i=0; i<shapes.size(); i++)
         {
            sh = (SKBaseShape)shapes.get(i);
            switch(sh.ShapeTypeID)
            {
               case 3:
                  SKLineShape line = (SKLineShape)sh;
                        //Write TypeID based if Segment,Ray,Line
                  switch (line.pt1.pointType+line.pt2.pointType)
                  {
                     case 0:dataInt[indexInt++]=3; //Segment
                           //System.out.println("typeID"+3);
                        break;
                     case 1: dataInt[indexInt++]=2; //Ray
                        break;
                     case 2: dataInt[indexInt++]=1; //Line (infinite)
                  }
                  break;
               default:
                  dataInt[indexInt++]=sh.ShapeTypeID;
                  break;
            }
            dataInt[indexInt++]=sh.ID;	
            switch (sh.ShapeTypeID)
            {
            
               case 0:
                  {
                     dataDouble[indexDbl++]=(double)sh.getShapeX();
                     dataDouble[indexDbl++]=(double)sh.getShapeY();
                  }
                  break;
               case 3 : 
                  { //SKLineShape
                        //p.writeInt(888);
                     SKLineShape line = (SKLineShape)sh;
                     dataDouble[indexDbl++]=(double)line.pt1.getShapeX();
                     dataDouble[indexDbl++]=(double)line.pt1.getShapeY();
                     dataDouble[indexDbl++]=(double)line.pt2.getShapeX();
                     dataDouble[indexDbl++]=(double)line.pt2.getShapeY();
                     dataDouble[indexDbl++]=line.length;
                        //p.writeInt(999);
                  }
                  break;
               case 4 : 
                  { //SKCircleShape
                     dataDouble[indexDbl++]=(double)((SKCircleShape)sh).center.getShapeX();
                     dataDouble[indexDbl++]=(double)((SKCircleShape)sh).center.getShapeY() ;
                     dataDouble[indexDbl++]=((SKCircleShape)sh).radius ;
                  }
                  break;
               case 5:
                  {
                     dataDouble[indexDbl++]=(double)((SKArcShape)sh).center.getShapeX();
                     dataDouble[indexDbl++]=(double)((SKArcShape)sh).center.getShapeY();
                     dataDouble[indexDbl++]=((SKArcShape)sh).radius;
                     dataDouble[indexDbl++]=((SKArcShape)sh).angle;
                  }
                  break;
               default: 
                  break;
            
            }
         
         }
      
      //Signify end of shape list
         dataInt[indexInt++]=-1;
      //p.writeInt(-1);
      
      //Write constraint list
         SKBaseConstraint con;
         SKConstraintArray Cons= new SKConstraintArray(100);
         for (i=0; i<shapes.size(); i++)
         {
            sh = (SKBaseShape)shapes.get(i);
            for (h=0; h<sh.ConstraintList.size(); h++)
            {
               con = sh.ConstraintList.get(h);
               if (con.isPrimaryShape(sh) && Cons.indexOf(con)==-1)
               {
                  Cons.add(con);
                  dataInt[indexInt++]=con.typeID;
                  //p.writeInt(con.typeID);
                  dataInt[indexInt++]=con.ID;
                  //p.writeInt(con.ID);
                  dataInt[indexInt++]=con.ShapeList.size();
                  //p.writeInt(con.ShapeList.size());
                  if (con instanceof SKNormalConstraint)
                  { //CAN consist of specific subshapes
                     for (g=0; g<con.ShapeList.size(); g++)
                     {
                        tmp = con.ShapeList.get(g);
                        dataInt[indexInt++]=tmp.ID;
                        //p.writeInt(tmp.ID);
                        dataInt[indexInt++]=con.getConInfo(tmp,0);
                        //p.writeInt(con.getConInfo(tmp,0));
                     }
                  
                     switch (con.typeID)
                     {
                        case 0: dataDouble[indexDbl++]=((SKDistanceConstraint)con).distance;
                           //p.writeDouble( ((SKDistanceConstraint)con).distance );
                           break;
                        case 4: dataDouble[indexDbl++]=((SKAngleConstraint)con).angle;
                        //p.writeDouble( ((SKAngleConstraint)con).angle );
                     }
                  }
                  else
                  { //Cannot consist of specific subshapes
                     for (g=0; g<con.ShapeList.size(); g++)
                        dataInt[indexInt++]=con.ShapeList.get(g).ID;
                     //p.writeInt(con.ShapeList.get(g).ID);
                  }
               }
            }
         
         }
      
      //Signify end of constraint list
         dataInt[indexInt++]=-1;
      //p.writeInt(-1);
      
      //Write group list
         indexInt=groupRoot.saveToUTUFile(indexInt, dataInt);
      
      //Signify end of group list (and file)
         dataInt[indexInt++]=-1;
      //p.writeInt(-1);
         utuJava utuDriver = new utuJava();
         utuDriver.utuC(dataInt, dataDouble);
      
      }
   
      public static float distance(int x1, int y1, int x2, int y2)
      
      
      
      {
         return (float)java.lang.Math.sqrt(java.lang.Math.pow((double)(x1-x2),2)+java.lang.Math.pow((double)(y1-y2),2));
      }
   
   }
