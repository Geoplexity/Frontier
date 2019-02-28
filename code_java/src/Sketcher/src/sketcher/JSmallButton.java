/**
 * Title:        JSmallButton
 * Description:  Implements a small button for use with JToolBar
 * @version 1.0
 */
   package sketcher;

   import java.awt.event.*;
   import javax.swing.JButton;
   import javax.swing.border.Border;
   import javax.swing.border.BevelBorder;
   import javax.swing.border.EmptyBorder;
   import javax.swing.Action;
   import javax.swing.Icon;
   import java.awt.Insets;


   public class JSmallButton extends JButton implements MouseListener
   {
      protected Border m_raised;
      protected Border m_lowered;
      protected Border m_inactive;
   
      public JSmallButton(Action act, String tip)
      {
         super((Icon)act.getValue(Action.SMALL_ICON));
         m_raised = new BevelBorder(BevelBorder.RAISED);
         m_lowered = new BevelBorder(BevelBorder.LOWERED);
         m_inactive = new EmptyBorder(2, 2, 2, 2);
      
         setBorder(m_inactive);
         setMargin(new Insets(1,1,1,1));
         setToolTipText(tip);
      
         addActionListener(act);
         addMouseListener(this);
      
         setRequestFocusEnabled(false);
      }
   
      public JSmallButton()
      {
         super();
         m_raised = new BevelBorder(BevelBorder.RAISED);
         m_lowered = new BevelBorder(BevelBorder.LOWERED);
         m_inactive = new EmptyBorder(2, 2, 2, 2);
      
         setBorder(m_inactive);
         setMargin(new Insets(1,1,1,1));
      //setToolTipText(tip);
      
      //addActionListener(act);
         addMouseListener(this);
      
         setRequestFocusEnabled(false);
      }
   
      public float getAlignmentY() { 
         return 0.5f; }
   
      public void mousePressed(MouseEvent e)
      {
         setBorder(m_lowered);
      }
   
      public void mouseReleased(MouseEvent e)
      {
         setBorder(m_inactive);
      }
   
      public void mouseClicked(MouseEvent e) {
      }
   
      public void mouseEntered(MouseEvent e)
      {
         setBorder(m_raised);
      }
   
      public void mouseExited(MouseEvent e)
      {
         setBorder(m_inactive);
      }
   
   }