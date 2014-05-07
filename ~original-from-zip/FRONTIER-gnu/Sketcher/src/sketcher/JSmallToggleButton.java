
/**
 * Title:        JSmallToggleButton
 * Description:  Small toggle button for use with JToolBar
 * @version 1.0
 */package sketcher;
   import java.awt.event.*;
   import javax.swing.JToggleButton;
   import javax.swing.border.Border;
   import javax.swing.border.BevelBorder;
   import javax.swing.border.EmptyBorder;
   import javax.swing.ImageIcon;
   import java.awt.Insets;

   public class JSmallToggleButton extends JToggleButton implements ItemListener
   {
      protected Border m_raised;
      protected Border m_lowered;
   
      public JSmallToggleButton(boolean selected, ImageIcon imgUnselected, ImageIcon imgSelected, String tip)
      {
         super(imgUnselected, selected);
         setHorizontalAlignment(CENTER);
         setBorderPainted(true);
      
         m_raised = new BevelBorder(BevelBorder.RAISED);
         m_lowered = new BevelBorder(BevelBorder.LOWERED);
      
         setBorder(selected ? m_lowered : m_raised);
         setMargin(new Insets(1,1,1,1));
         setToolTipText(tip);
         setRequestFocusEnabled(false);
         setSelectedIcon(imgSelected);
      
         addItemListener(this);
      }
   
      public JSmallToggleButton()
      {
         super();
         setHorizontalAlignment(CENTER);
         setBorderPainted(true);
      
         m_raised = new BevelBorder(BevelBorder.RAISED);
         m_lowered = new BevelBorder(BevelBorder.LOWERED);
      
         setBorder(m_raised);
         setMargin(new Insets(1,1,1,1));
      //setToolTipText(tip);
         setRequestFocusEnabled(false);
      //setSelectedIcon(imgSelected);
      
         addItemListener(this);
      }
   
      public float getAlignmentY() { 
         return 0.5f; }
   
      public void itemStateChanged(ItemEvent e)
      {
         setBorder(isSelected() ? m_lowered : m_raised);
      }
   
   }