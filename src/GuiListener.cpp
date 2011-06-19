#include "precompiled.h"
#include "GuiListener.h"

GuiListener::GuiListener(MyGUI::Gui* m):mGUI(m), mouseX(0), mouseY(0) {}
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
   MyGUI::Char translateWin32Text(MyGUI::KeyCode kc)
   {
      static WCHAR deadKey = 0;

      BYTE keyState[256];
      HKL  layout = GetKeyboardLayout(0);
      if ( GetKeyboardState(keyState) == 0 )
         return 0;

      int code = *((int*)&kc);
      unsigned int vk = MapVirtualKeyEx((UINT)code, 3, layout);
      if ( vk == 0 )
         return 0;

      WCHAR buff[3] = { 0, 0, 0 };
      int ascii = ToUnicodeEx(vk, (UINT)code, keyState, buff, 3, 0, layout);
      if (ascii == 1 && deadKey != '\0' )
      {
         // A dead key is stored and we have just converted a character key
         // Combine the two into a single character
         WCHAR wcBuff[3] = { buff[0], deadKey, '\0' };
         WCHAR out[3];

         deadKey = '\0';
         if(FoldStringW(MAP_PRECOMPOSED, (LPWSTR)wcBuff, 3, (LPWSTR)out, 3))
            return out[0];
      }
      else if (ascii == 1)
      {
         // We have a single character
         deadKey = '\0';
         return buff[0];
      }
      else if(ascii == 2)
      {
         // Convert a non-combining diacritical mark into a combining diacritical mark
         // Combining versions range from 0x300 to 0x36F; only 5 (for French) have been mapped below
         // http://www.fileformat.info/info/unicode/block/combining_diacritical_marks/images.htm
         switch(buff[0])   {
         case 0x5E: // Circumflex accent: â
            deadKey = 0x302; break;
         case 0x60: // Grave accent: à
            deadKey = 0x300; break;
         case 0xA8: // Diaeresis: ü
            deadKey = 0x308; break;
         case 0xB4: // Acute accent: é
            deadKey = 0x301; break;
         case 0xB8: // Cedilla: ç
            deadKey = 0x327; break;
         default:
            deadKey = buff[0]; break;
         }
      }

      return 0;
   }
#endif

bool GuiListener::keyPressed(const Ogre::FrameEvent &evt, const OIS::KeyEvent &arg )
{
    MyGUI::Char text = (MyGUI::Char)arg.text;
      MyGUI::KeyCode key = MyGUI::KeyCode::Enum(arg.key);
      int scan_code = key.toValue();

      if (scan_code > 70 && scan_code < 84)
      {
         static MyGUI::Char nums[13] = { 55, 56, 57, 45, 52, 53, 54, 43, 49, 50, 51, 48, 46 };
         text = nums[scan_code-71];
      }
      else if (key == MyGUI::KeyCode::Divide)
      {
         text = '/';
      }
      else
      {
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
         text = translateWin32Text(key);
#endif
      }

      MyGUI::InputManager::getInstance().injectKeyPress(key, text);
      return true;
}
bool GuiListener::keyReleased(const Ogre::FrameEvent &evt, const OIS::KeyEvent &arg )
{
    mGUI->injectKeyRelease(MyGUI::KeyCode::Enum(arg.key));
    return true;
}
bool GuiListener::mouseMoved(const Ogre::FrameEvent &evt, const OIS::MouseEvent &arg )
{
    float minX = float(0);
    float maxX = float(arg.state.width);
    float minY = float(0);
    float maxY = float(arg.state.height);
    mouseX += /*0.00005 * arg.state.width */ arg.state.X.rel;
    mouseY += /*0.00005 * arg.state.height */ arg.state.Y.rel;

    mouseX = std::max(mouseX, minX);
    mouseX = std::min(mouseX, maxX);
    mouseY = std::max(mouseY, minY);
    mouseY = std::min(mouseY, maxY);
    mGUI->injectMouseMove(mouseX, mouseY, arg.state.Z.abs);
    return true;
}
bool GuiListener::mousePressed(const Ogre::FrameEvent &evt,const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    mGUI->injectMousePress(mouseX, mouseY, MyGUI::MouseButton::Enum(id));
    return true;
}
bool GuiListener::mouseReleased(const Ogre::FrameEvent &evt, const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    mGUI->injectMouseRelease(mouseX, mouseY, MyGUI::MouseButton::Enum(id));
    return true;
}

