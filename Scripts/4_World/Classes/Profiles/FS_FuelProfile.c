class ITZ_FS_FuelProfile
{
    string m_FuelType;

    float m_Capacity;
    float m_Min;
    float m_Max;
    
    float m_Quantity;

    [NonSerialized()]
    bool m_SynchDirty;

    void AddQuantity(float quantity)
    {
        if((m_Quantity + quantity) >= m_Capacity)
            m_Quantity = m_Capacity;
        else
            m_Quantity += quantity;
    }

    void RemoveQuantity(float quantity)
    {
        if((m_Quantity - quantity) < 0)
            m_Quantity = 0;
        else
            m_Quantity -= quantity;
    }

    bool HasFuel()
    {
       return m_Quantity > 0;
    }

    string GetFuelType()
    {
        return m_FuelType;
    }

    int GetLiquidType()
    {
        return ITZ_FS_Constants.GetFuelTypeFromNamedType(m_FuelType);
    }

    void SetLiquidType(int liquidType)
    {
        m_FuelType = ITZ_FS_Constants.GetNamedTypeFromFuelType(liquidType);
    }

    void SetLiquidType(string fuelType)
    {
        m_FuelType = fuelType;
    }

    float GetCapacity()
    {
        return m_Capacity;
    }

    void SetCapacity(float capacity)
    {
        m_Capacity = capacity;
    }

    float GetMin()
    {
        return m_Min;
    }

    void SetMin(float min)
    {
        m_Min = min;
    }

    float GetMax()
    {
        return m_Max;
    }

    void SetMax(float max)
    {
        m_Max = max;
    }

    float GetQuantity()
    {
        return m_Quantity;
    }

    void SetQuantity(float quantity)
    {
        m_Quantity = quantity;
    }

    bool IsFull()
    {
        return m_Quantity >= m_Capacity;
    }

    bool GetSynchDirty()
    {
        return m_SynchDirty;
    }

    void SetSynchDirty(bool dirty = true)
    {
        m_SynchDirty = dirty;
    }
}