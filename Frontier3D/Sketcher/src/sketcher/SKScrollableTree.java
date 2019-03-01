   package sketcher;

   import java.awt.*;
   import java.awt.event.*;
   import javax.swing.*;
   import javax.swing.border.*;

   public class SKScrollableTree extends scrollableFrame
   {
      public Container container = null;
      public SKGTNArray Tree = null;
      public SKGroupTreeNode Root = null;
      public int[] flag;
      public int ID;
      public SKMainFrame temp = null;
      public SKTextArray text = new SKTextArray(2);
      public SKEdgeArray edges = null;
      private Color black = Color.black;
      private Color magenta = Color.magenta;
      private Color red = Color.red;
      private Color blue = Color.blue;
   
      public SKScrollableTree(Container cont,SKMainFrame frameMain, SKGTNArray t, SKGroupTreeNode r, int active, int[] fin)
      {
         super(frameMain);
         temp = frameMain;
         container = cont;
         Tree = t;
         Root = r;
         ID = active;
         flag = fin;
         try 
         {
            jbInit();
            setSize(500,350);
            /*Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
            Dimension d = getSize();
            int x = (screen.width - d.width) >> 1;   
            int y = (screen.height - d.height) >> 1;
            setLocation(x, y);*/
         }
            catch(Exception e)
            {      
               e.printStackTrace();
            }
      
      }
      public void jbInit()
      {
         addMouseListener(
                            new java.awt.event.MouseAdapter()
                            {
                            
                               public void mouseClicked(MouseEvent e)
                               {
                                  if (e.isControlDown())
                                  { //Draw the original tree
                                     if(container instanceof TreeDialog)
                                        ((TreeDialog)container).drawOriginalTree();
                                     else
                                        ((TreeFrame)container).drawOriginalTree();
                                     repaint();
                                  }
                                  else
                                  {
                                     if(e.getComponent() != null && doHitTest(e.getX(),e.getY()) != null )
                                     {
                                        if(container instanceof TreeDialog)
                                        {
                                           ((TreeDialog)container).mouseOverItem =(SKText)doHitTest(e.getX(),e.getY());
                                           ((TreeDialog)container).reDrawTree();
                                        }
                                        else
                                        {
                                           ((TreeFrame)container).mouseOverItem =(SKText)doHitTest(e.getX(),e.getY());
                                           ((TreeFrame)container).reDrawTree();
                                        }
                                        repaint();
                                     }
                                  } 
                               
                               
                               }
                            
                               public void mouseReleased(MouseEvent e)
                               {
                               }
                            
                            });
         addMouseMotionListener(
                              
                                 new java.awt.event.MouseMotionAdapter()
                                 
                                 {
                                 
                                    public void mouseMoved(MouseEvent e)
                                    
                                    {
                                       if(container instanceof TreeDialog)
                                          ((TreeDialog)container).mouseMoved(e);
                                    }
                                 });
      
      }
      public void paint(Graphics g)
      {
         super.paint(g);
         for(int i = 0; i<getComponentCount() ;i++)
            ((SKText)getComponent(i)).paintComponent(g);
         g.setColor(black);
         for(int i =0; i<edges.size(); i++)
            g.drawLine(edges.get(i).x1,edges.get(i).y1,edges.get(i).x2,edges.get(i).y2);
      }
      public void drawTree()
      {
         SKGTNArray tree = Tree;
         SKGroupTreeNode root = Root;
      
         SKGTNArray tempArray = new SKGTNArray(1);
         SKGTNArray finalArray = new SKGTNArray(1);
         SKGTNArray rootArray = new SKGTNArray(1);
         SKGTNArray children = new SKGTNArray(1);
         SKShapeArray exshapes = new SKShapeArray(1);
      
         edges = new SKEdgeArray(2);
      
         int center = 0;
         int x1,y1,x2,y2,total=0;
         text.clear();	
         this.removeAll();
         tree.copyArrayTo(tempArray);
         for(int i=0; i< tempArray.size();i++)
         {
            if(tempArray.get(i).parents.size()==0 && tempArray.get(i).parent==null)
               rootArray.add(tempArray.get(i));
         }
      
         SKGTNArray tempNodes = new SKGTNArray(rootArray.size());
         SKGTNArray temproots = new SKGTNArray(rootArray.size());
      
         rootArray.copyArrayTo(temproots);
         for(int i=0; i<temproots.size(); i++)
         {
            SKGroups.setLevels(tempArray);
            SKGroups.separateGroups(temproots.get(i),tempNodes);
            int rootsize= temproots.get(i).shapes.size();
            for(int a=0; a<tempNodes.size(); a++)
            {
               int diff=rootsize-tempNodes.get(a).shapes.size();
               tempNodes.get(a).width=((temproots.get(i).width)-50*diff);
            }
            tempNodes.clear();  
         }
         for(int i=0; i< rootArray.size();i++)
         {
            rootArray.get(i).x= (this.getWidth()/(rootArray.size()+1))*(i+1);
            rootArray.get(i).y=50;
         }		
         finalArray.clear();
      
      /****Add all the root nodes into the rootArray****/
         if(rootArray.size()>0)
            for(int i=0;i<rootArray.size();i++)
               finalArray.add(rootArray.get(i));
      
         while(rootArray.size()>0)
         {
            SKGTNArray tempRootArray = new SKGTNArray(100);
            for (int a=0; a<rootArray.size(); a++)
               for (int b=0; b<rootArray.get(a).children.size(); b++)
               {
                  children.add(rootArray.get(a).children.get(b));
                  tempRootArray.add(rootArray.get(a).children.get(b));
               }
            if(tempRootArray.size()>0)tempRootArray.copyArrayTo(rootArray);
            else rootArray.clear();
         }
      /****Add all the other nodes into the rootArray, in sequence****/
         if(children.size()>0)
            for(int i=0;i<children.size();i++)
               finalArray.add(children.get(i));
      
         for(int i=0; i<finalArray.size();i++)
         {
            exshapes.clear();
            if(finalArray.get(i).children.size()>0)
            {
               SKShapeArray childshapes = new SKShapeArray(100);
            /**** create an array of shapes in all the groups****/
               for(int c = 0; c <finalArray.get(i).children.size();c++)
                  for(int b=0; b<finalArray.get(i).children.get(c).shapes.size();b++)
                     if(   childshapes.indexOf( finalArray.get(i).children.get(c).shapes.get(b) ) ==  -1 )
                        childshapes.add(finalArray.get(i).children.get(c).shapes.get(b));
            
               if(finalArray.get(i).shapes.size()>childshapes.size())
               {
                  for(int c=0; c<finalArray.get(i).shapes.size(); c++)
                     if(childshapes.indexOf(finalArray.get(i).shapes.get(c))==-1)
                        exshapes.add(finalArray.get(i).shapes.get(c));
                  total = finalArray.get(i).children.size()+exshapes.size();
               }
            
               for(int a=0; a<finalArray.get(i).children.size(); a++)
               {  
                  finalArray.get(i).children.get(a).y= finalArray.get(i).y + 45;
                  if(exshapes.size()==0)
                     finalArray.get(i).children.get(a).x=(finalArray.get(i).x-(finalArray.get(i).width/2))+((finalArray.get(i).width/(finalArray.get(i).children.size()+1) )*(a+1));
                  else
                     finalArray.get(i).children.get(a).x=(finalArray.get(i).x-(finalArray.get(i).width/2))+((finalArray.get(i).width/(total+1))*(a+1));
               }
            
               if(exshapes.size()>0)
                  for(int b=0;b<exshapes.size();b++)
                  {
                     x1=finalArray.get(i).x;
                     y1=finalArray.get(i).y;
                     y2=finalArray.get(i).y+45;
                     int exsx=(finalArray.get(i).x-(finalArray.get(i).width/2))+((finalArray.get(i).width/(total+1))*(finalArray.get(i).children.size()+b+1));
                     SKEdge edge = new SKEdge(x1,y1,exsx,y2);
                     edges.add(edge);
                     SKText shapeName = new SKText(this,container,exshapes.get(b).Name.charAt(0)+""+exshapes.get(b).ID,exsx,y2);
                     if(text.findByValue(shapeName.value) == null)
                        text.add(shapeName);
                     shapeName.repaint();
                  }
            }
         }
         for(int i=0; i<finalArray.size();i++)
            for(int a = 0; a <finalArray.get(i).children.size();a++)
            {
               x1=finalArray.get(i).x;
               y1=finalArray.get(i).y;
               x2=finalArray.get(i).children.get(a).x;
               y2=finalArray.get(i).children.get(a).y;
               SKEdge edge = new SKEdge(x1,y1,x2,y2);
               edges.add(edge);
            }
         for(int i=0; i<finalArray.size(); i++)
         {
            SKText groupName = null;		
            if(finalArray.get(i).parents.size()==0 && finalArray.get(i).parent==null)
               groupName = new SKText(this,container,finalArray.get(i).name,finalArray.get(i).x,finalArray.get(i).y);
            else if(ID==0)
               groupName = new SKText(this,container,"G"+finalArray.get(i).ID,finalArray.get(i).x,finalArray.get(i).y);
            else 
               groupName = new SKText(this,container,"C"+finalArray.get(i).ID,finalArray.get(i).x,finalArray.get(i).y);
         
            if(groupName!=null && !finalArray.get(i).matchingIP.equals(""))
            {
               groupName.setMatchingIPName(finalArray.get(i).matchingIP);
            }
         
         
         
            if(finalArray.get(i).ID==ID)
               groupName.setColor(magenta);
            else if (flag[Tree.indexOf(finalArray.get(i))]==1) 
               groupName.setColor(blue);
            else groupName.setColor(black);
         
            if(groupName!=null)
            {
               groupName.repaint();
               if(text.findByValue(groupName.value) == null)
                  text.add(groupName);
            }
         
            if(finalArray.get(i).children.size()==0)
            {
               int sx, sy;
               for  (int a=0; a<finalArray.get(i).shapes.size(); a++)
               {
                  sy= finalArray.get(i).y +45;
                  if(finalArray.get(i).shapes.size()==1)  sx=finalArray.get(i).x;
                  else sx= (finalArray.get(i).x-(finalArray.get(i).width/4))+( ((finalArray.get(i).width/2)/(finalArray.get(i).shapes.size()+1) )*(a+1));
                  SKEdge edge = new SKEdge(finalArray.get(i).x,finalArray.get(i).y,sx,sy);
                  edges.add(edge);
                  SKText shapeName = null;
                  if(finalArray.get(i).shapes.get(a)!=null)
                     shapeName = new SKText(this,container,finalArray.get(i).shapes.get(a).Name.charAt(0)+""+finalArray.get(i).shapes.get(a).ID,sx,sy);
                  else
                     shapeName = new SKText(this,container,"U",sx,sy);
                  shapeName.setColor(red);
               
                  if(text.findByValue(shapeName.value) == null)
                     text.add(shapeName);
               }
            }
         }
      }
      public void addComponents()
      {
         for(int i=0 ;i <text.size(); i++)
            this.add(text.get(i));
      }
   
      public SKText doHitTest(int x, int y)
      {
         for(int i=0 ; i<text.size(); i++)
         {
            if(x>text.get(i).x-10 && x<text.get(i).x+15)
               if(y>text.get(i).y-10 && y<text.get(i).y+15)
                  return text.get(i);
         }
         return null;
      }
   }
