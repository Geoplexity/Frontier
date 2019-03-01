
/**
 * Title:        ImageComboBoxRenderer
 * Description:  Draws images alongside text in a JComboBox
 */
   package sketcher;

   import javax.swing.JLabel;
   import javax.swing.JList;
   import javax.swing.ListCellRenderer;
   import javax.swing.ImageIcon;
   import javax.swing.Icon;
   import java.awt.Insets;
   import javax.swing.border.*;
   import java.awt.Color;
   import java.awt.Component;
   import java.awt.Graphics;
   import java.io.File;

   public class ImageComboBoxRenderer extends JLabel implements ListCellRenderer
   {
      public static final int OFFSET = 16;
   
      String HomeDir;
   
      protected Color m_textSelectionColor = Color.white;
      protected Color m_textNonSelectionColor = Color.black;
      protected Color m_textNonselectableColor = Color.gray;
      protected Color m_bkSelectionColor = new Color(0, 0, 128);
      protected Color m_bkNonSelectionColor = Color.white;
      protected Color m_borderSelectionColor = Color.yellow;
   
      protected Color m_textColor;
      protected Color m_backColor;
   
      protected boolean m_hasFocus;
      protected Border[] m_borders;
   
      public ImageComboBoxRenderer()
      {
         super();
         m_textColor = m_textNonSelectionColor;
         m_backColor = m_bkNonSelectionColor;
         m_borders = new Border[20];
         for (int k=0; k < m_borders.length; k++)
            m_borders[k] = new EmptyBorder(0, OFFSET * k, 0, 0);
         setOpaque(false);
      
         File f = new File("");
         HomeDir = f.getAbsolutePath();
      }
   
      public Component getListCellRendererComponent(JList list, Object value, int row, boolean isSelected, boolean cellHasFocus)
      {
         if (value==null)
         {
            setText("");
            setIcon(null);
            return this;
         }
      
         setText(value.toString());
         boolean selectable = true;
      /*
      if (value instanceof ListData) {
      ListData ldata = (ListData)obj;
      selectable = ldata.isSelectable();
      int index = 0;
      if (row >= 0)    // no offset for editor (row=-1)
        index = ldata.getIndex();
      Border b = (index < m_borders.length ? m_borders[index] :
        new EmptyBorder(0, OFFSET * index, 0, 0));
      setBorder(b);
      }
      */
      //setIcon(shapeIcon);
         setIcon( new ImageIcon(HomeDir+ ((SKBaseShape)value).IconPath ) );
      
         setFont(list.getFont());
         m_textColor = (isSelected ? m_textSelectionColor :
                          (selectable ? m_textNonSelectionColor :
                          m_textNonselectableColor));
         m_backColor = (isSelected ? m_bkSelectionColor :
                       m_bkNonSelectionColor);
         m_hasFocus = cellHasFocus;
      
         return this;
      }
   
      public void paint(Graphics g)
      {
         Icon icon = getIcon();
         Border b = getBorder();
      
         g.setColor( Color.black );
         g.fillRect(0, 0, getWidth(), getHeight());
      
         g.setColor(m_backColor);
         int offset = 0;
         if(icon != null && getText() != null) {
            Insets ins = getInsets();
            offset = ins.left + icon.getIconWidth() + getIconTextGap()-4;
         }
         g.fillRect(offset, 0, getWidth() /*- 1*/ - offset,
                   getHeight() /*- 1*/);
      
         if (m_hasFocus) {
            g.setColor(Color.yellow);
            g.drawRect(offset, 0, getWidth()-1-offset, getHeight()-1);
         }
      
         setForeground(m_textColor);
         setBackground(m_backColor);
         super.paint(g);
      }
   }