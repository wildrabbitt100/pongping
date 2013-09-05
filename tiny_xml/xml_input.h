




#include "






bool XMLConfiguration::Save()
{
  LOG(INFO) << "Saving xml log file " << document.Value;
  bool success=document.SaveFile();
  LOG(INFO) << "Save success: " << success;
  return success;
}



bool XMLConfiguration::Load(const std::string& filename)
{
  LOG(INFO) << "Loading xml log file " << filename;
  bool success=document.LoadFile(filename);
  LOG(INFO) << "Loading success: " << success;
  return success;
}



std::string XMLConfiguration::GetValue(const std::string& section, const std::string& field, const std::string& defaultValue) 
{
  std::string value(defaultValue);
  TiXmlHandle docHandle( &document );
  TiXmlNode* fieldElement = docHandle.FirstChild(section).FirstChild( field ).ToElement();
  if ( fieldElement )
  {
    value=std::string(((TiXmlElement*)fieldElement)->GetText());
    VLOG(1) << "DEBUG: GetValue Success for " << section << "/" << field;
  }
  else
  {
    VLOG(1) << "DEBUG: GetValue Fail for " << section << "/" << field;
  }
  VLOG(1) << "Returning " << value;
  
  return value;
}



bool XMLConfiguration::SetValue(const std::string& section, const std::string& field, const std::string& value)
{
  TiXmlElement* fieldElement = document.FirstChildElement( section );
  if ( fieldElement )
  {
    TiXmlElement* config=fieldElement->FirstChildElement( field );
    if(config)
      fieldElement->SetValue(value);
    else
    {
      //create element
      TiXmlElement e(field);
      e.SetValue(value);
      fieldElement->LinkEndChild(&e);
    }
    return true;
  }
  else
    return false;
}