class ITZ_FS_XMLHelper
{
    static string GetAttributeString(CF_XML_Tag tag, string name, string defaultValue = "")
	{
		auto attr = tag.GetAttribute(name);
		if (attr)
			return attr.ValueAsString();

		return defaultValue;
	}

    static float GetAttributeFloat(CF_XML_Tag tag, string name, float defaultValue = 0)
	{
		auto attr = tag.GetAttribute(name);
		if (attr)
            return attr.ValueAsFloat();
			
		return defaultValue;
	}

	static int GetAttributeInt(CF_XML_Tag tag, string name, int defaultValue = 0)
	{
		auto attr = tag.GetAttribute(name);
		if (attr)
            return attr.ValueAsInt();
			
		return defaultValue;
	}

	static bool GetAttributeBool(CF_XML_Tag tag, string name, bool defaultValue = false)
	{
		auto attr = tag.GetAttribute(name);
		if (attr)
            return attr.ValueAsBool();
			
		return defaultValue;
	}

    static int GetTagContentInt(CF_XML_Tag tag, string type, int defaultValue = 0)
	{
		auto children = tag.GetTag(type);
		if (children.Count() > 0)
			return children[0].GetContent().GetContent().ToInt();

		return defaultValue;
	}

    static float GetTagContentFloat(CF_XML_Tag tag, string type, float defaultValue = 0)
	{
		auto children = tag.GetTag(type);
		if (children.Count() > 0)
			return children[0].GetContent().GetContent().ToFloat();
		return defaultValue;
	}

    static string GetTagContentAttributeString(CF_XML_Tag tag, string type, string attributeName, string defaultValue = "")
	{
		auto children = tag.GetTag(type);
		if (children.Count() > 0)
			return GetAttributeString(children[0], attributeName, defaultValue);
		return defaultValue;
	}
}