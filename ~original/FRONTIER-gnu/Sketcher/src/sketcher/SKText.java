   //Changed to incorporate the dialog of the scrollable tree. // search for comments to know where the changes have been made.


   package sketcher;

   import java.awt.*;
   import java.awt.event.*;
   import javax.swing.*;
   import javax.swing.border.*;

   public class SKText extends JComponent implements SKItemInterface
   {
      public String value="";
      public String matchingIPName="";
      public int x,y;
      public Container owner = null;
      public Color color = Color.blue;
   
      private Color black = Color.black;
      private Color magenta = Color.magenta;
      private Color red = Color.red;
      private Color blue = Color.blue;
      private Color white = Color.white;
      public SKText(JPanel panel, Container frame, String text, int xPosition, int yPosition)
      {
         value = text;
         x = xPosition;
         y = yPosition;
         owner = frame;
      //  addMouseHandlers();
         panel.add(this);
      }
      public void paintComponent(Graphics g)
      {
         g.setColor(color);
         g.drawString(value,x,y);
         if(!matchingIPName.equals(""))
            g.fillOval(x,y,6,6);
      }
      public String getString()
      {
         return value;
      }
      public boolean doHitTest(int x, int y)
      {
         return false;
      }
      public SKItemInterface getClonedItem()
      {
         return null;
      }
      public void severTies()
      {}
      public void setColor(Color c)
      {
         color = c;
         repaint();
      }
      public void setMatchingIPName(String s)
      {
         matchingIPName = s;
      }
      public void addMouseHandlers()
      {
         this.addMouseListener(
                                 new MouseListener()
                                 {
                                    public void mouseClicked(MouseEvent e)
                                    {
                                       System.out.println("111 clicked on "+value);
                                       if (e.isControlDown())
                                       { //Draw the original tree
                                          if(owner instanceof TreeFrame)
                                             ((TreeFrame)owner).drawOriginalTree();
                                          else if (owner instanceof TreeDialog)
                                             ((TreeDialog)owner).drawOriginalTree();
// 					  else if(owner instanceOf SKBifurFrame2d) // Extra elseif added for that
// 						((SKBifurFrame2d)owner).drawOriginalTree();
                                       }
                                       else
                                       {
                                       //if(e.getComponent() != null)
                                       //{
                                          if(owner instanceof TreeFrame)
                                          {
                                             ((TreeFrame)owner).mouseOverItem = (SKText)e.getComponent();
                                             ((TreeFrame)owner).reDrawTree();
                                          }
                                          else if (owner instanceof TreeDialog)
                                          {
                                             ((TreeDialog)owner).mouseOverItem = (SKText)e.getComponent();
                                             ((TreeDialog)owner).reDrawTree();
                                          }
// 					  else if(owner instanceOf SKBifurFrame2d)
// 					  {
// 					  	((SKBifurFrame2d)owner).mouseOverItem = (SKText) e.getComponent();
// 						((SKBifurFrame2d)owner).reDrawTree();
// 					  }
                                          owner.repaint();
                                       //}
                                       }
                                    }
                                    public void mouseReleased(MouseEvent e){
                                    }
                                    public void mousePressed(MouseEvent e) {
                                    }
                                    public void mouseExited(MouseEvent e) {
                                    }
                                    public void mouseEntered(MouseEvent e) {
                                    }
                                 }	
                              );
      
         this.addMouseMotionListener(
                              
                                 new MouseMotionListener()
                                 {
                                 
                                    public void mouseDragged(MouseEvent e)
                                    {}
                                    public void mouseMoved(MouseEvent e)  
                                    { //Set correct mouse cursor
                                       color = white;
                                       repaint();
                                    }
                                 } );
      
      }
   }
