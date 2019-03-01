/**
 * Title:        JStatusBar
 * Description:  Implements a basic windows-like statusbar
 * @version 1.0
 */
   package sketcher;

   import javax.swing.JPanel;
   import java.awt.Graphics;
   import java.util.ArrayList;
   import java.awt.Color;

   public class JStatusBar extends JPanel
   {
      static final int ALIGN_LEFT = 0;
      static final int ALIGN_CENTER = 1;
      static final int ALIGN_RIGHT = 2;
      private ArrayList Panels = new ArrayList();
   
      public void paintComponent(Graphics g)
      {
         super.paintComponent(g);
      
         int tmp=0;
         int strt;
         for (int i=0; i<Panels.size(); i++)
         {
            JStatusBarPanel sbp = (JStatusBarPanel)Panels.get(i);
         
            int x = (int)g.getFontMetrics().getStringBounds(sbp.text,g).getWidth();
         
            switch (sbp.align)
            {
               case ALIGN_CENTER:
                  { strt=(int)((sbp.size/2)-(g.getFontMetrics().getStringBounds(sbp.text,g).getWidth()/2))+2; }
                  break;
               case ALIGN_RIGHT: 
                  { strt=(int)(sbp.size - g.getFontMetrics().getStringBounds(sbp.text,g).getWidth()); }
                  break;
               default :
                  { strt=3; }
            }
         
            if (i==0)
            {
               g.drawString( sbp.text,strt,11 );
               tmp=sbp.size+6;
            }
            else
            {
            //Draw bevel
               g.setColor(Color.white);
               g.drawLine( tmp-3,0,tmp-3,getHeight() );
               g.drawLine( tmp-2,0,tmp-2,getHeight() );
               g.setColor(Color.gray);
               g.drawLine( tmp-1,0,tmp-1,getHeight() );
               g.setColor(Color.darkGray);
               g.drawLine( tmp,0,tmp,getHeight() );
            
            //Draw text
               g.setColor(Color.black);
               g.drawString( sbp.text,tmp+strt,11 );
               tmp = tmp+sbp.size+6;
            }
         }
      }
   
      public void addPanel(String st, int size, int align)
      {
         Panels.add(new JStatusBarPanel(st,size,align) );
      
         repaint();
      }
   
      public void insertPanel(String st, int sz, int align, int pos)
      {
         Panels.add( pos, new JStatusBarPanel(st,sz,align) );
      
         repaint();
      }
   
      public void removePanel(int pos)
      {
         Panels.remove(pos);
      
         repaint();
      }
   
      public int getPanelSizeAt(int pos)
      {
         return ((JStatusBarPanel)Panels.get(pos)).size;
      }
   
      public String getPanelTextAt(int pos)
      {
         return ((JStatusBarPanel)Panels.get(pos)).text;
      }
   
      public int getPanelCount()
      {
         return Panels.size();
      }
   
      public void updatePanel(String st, int sz, int algn, int pos)
      {
         JStatusBarPanel sbp = (JStatusBarPanel)Panels.get(pos);
      
         sbp.text=st;
         sbp.size=sz;
         sbp.align=algn;
      
         repaint();
      }
   
      public void updatePanelText(String st, int pos)
      {
      
         ((JStatusBarPanel)Panels.get(pos)).text=st;
         repaint();
      }
   }