
/**
 * Title:        JSelectionPanel
 * Description:  Extends a JPanel to allow for drawing a selection rect
 */
   package sketcher;

   import javax.swing.JPanel;
   import java.awt.Graphics;
   import java.awt.Graphics2D;
   import java.awt.Stroke;
   import java.awt.BasicStroke;
   import java.awt.Color;
   import java.awt.geom.Rectangle2D;

   public class JSelectionPanel extends JPanel
   {
      public SKMainFrame frameMain;
      public boolean DrawSelRect = false;
      public boolean DrawConstraints = true;
      public int     StartX,StartY,EndX,EndY;
      public float   scale=1;
   
      public JSelectionPanel(SKMainFrame mainFrame)
      {
         super();
         frameMain = mainFrame;
      }
      public void paint(Graphics g)
      {
         super.paint(g);
         for(int i=0; i<getComponentCount(); i++)
            ((SKBaseShape)this.getComponent(i)).paintConstraints(g);
      }
      public void paintComponent(Graphics g)
      {
         super.paintComponent(g);
         if (DrawConstraints)
         {
            for (int i=0; i<getComponentCount(); i++)
            {
               ((SKBaseShape)getComponent(i)).getSelectable().paintConstraints(g);
               ((SKBaseShape)getComponent(i)).paintSpecial(g);
            }
         }
         else
         {//System.out.println(getComponentCount());
            for (int i=0; i<getComponentCount(); i++)
               ((SKBaseShape)getComponent(i)).paintSpecial(g);
         }
      
      //Draw group rects?
         if (frameMain.drawGroupRects)
         {
            SKGroupTreeNode node;
            for (int i=0; i<frameMain.selectedGroups.size(); i++)
            {
               node = frameMain.selectedGroups.get(i);
               if (node.groupRect.w == 0)  node.groupRect = SKGroups.getRectForGroup(node);
            
               g.drawRect( node.groupRect.x-2, node.groupRect.y-2, node.groupRect.w+8, node.groupRect.h+8 );
            }
         }
      
      //Draw selection rect
         if (DrawSelRect)
         {
            Graphics2D g2d = (Graphics2D)g;
         
            Stroke st = g2d.getStroke();
            g2d.setStroke( new BasicStroke(1,BasicStroke.CAP_BUTT,BasicStroke.JOIN_BEVEL,0,new float[] {3,3},0) );
            g2d.setColor(Color.black);
         
         //Setup to allow for seleciton rect to draw properly in all quadrants
            int sx,sy,ex,ey;
            if (StartX<EndX)
            {
               sx = StartX;
               ex = EndX;
            }
            else
            {
               sx = EndX;
               ex = StartX;
            }
            if (StartY<EndY)
            {
               sy = StartY;
               ey = EndY;
            }
            else
            {
               sy = EndY;
               ey = StartY;
            }
         
            g2d.drawRect(sx,sy,ex-sx,ey-sy);
         
            g2d.setStroke( st );
         }
      }
     /* void drawNames(Graphics g)
      
      {
         int w=getWidth();
         int h=getHeight();
         for(int i=0; i<this.getComponentCount(); i++)
         {
            SKBaseShape sh= (SKBaseShape) this.getComponent(i);
            if((sh.getX()+80>0) && (sh.getX()+80<w))
               if((sh.getY()+80<h) && (sh.getY()+80>0))
            g.drawString(sh.Name.charAt(0)+""+sh.ID, sh.getX()+60, sh.getY()+85);}
      }*/
      void drawSt( Graphics g, SKShapeArray solvingShapes)
      
      {
         Color red = Color.red;
         int w=getWidth();
         int h=getHeight();
         for (int j=0; j<this.getComponentCount(); j++)
         {  //if(solvingShapes.get(i).ID==((SKBaseShape)panelShapeArea.getComponent(j)).ID);
            SKBaseShape solveShape = (SKBaseShape) this.getComponent(j);
            if(solvingShapes.findByID(solveShape.ID) !=null)
            {
               g.setColor(red);
               if((solveShape.getShapeX()+30>0) && (solveShape.getShapeY()+80>30))
                  if((solveShape.getShapeX()+30<w) && (solveShape.getShapeY()+80<h))
                     g.drawString(solveShape.Name.charAt(0)+""+solveShape.ID, solveShape.getShapeX()+30, solveShape.getShapeY()+80);
            }
         }
      }
   }
