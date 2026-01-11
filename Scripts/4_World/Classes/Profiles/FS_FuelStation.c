
class ITZ_FS_FuelStation
{
    string m_ID;
    vector m_Position;
    ref ITZ_FS_FuelStationVariables m_Variables;
    
    ref array<ref ITZ_FS_FuelProfile> m_Fuels;

    [NonSerialized()]
    bool m_SynchDirty;

    void AddFuelProfile(ITZ_FS_FuelProfile profile)
    {
        m_Fuels.Insert(profile);
    }

    string GetId()
    {
        return m_ID;
    }

    void SetId(string id)
    {
        m_ID = id;
    }

    vector GetPosition()
    {
        return m_Position;
    }

    void SetPosition(vector position)
    {
        m_Position = position;
    }

    array<ref ITZ_FS_FuelProfile> GetFuels()
    {
        return m_Fuels;
    }

    void SetFuels(array<ref ITZ_FS_FuelProfile> fuels)
    {
        m_Fuels = fuels;
    }

    bool HasFuel(int type)
    {
        foreach (ITZ_FS_FuelProfile profile: m_Fuels)
        {
            if (profile && profile.GetLiquidType() == type && profile.HasFuel())
            {
                return true;
            }
        }
        return false;
    }

    float GetCapacity(int type)
    {
        foreach (ITZ_FS_FuelProfile profile: m_Fuels)
        {
            if (profile && profile.GetLiquidType() == type)
            {
                return profile.GetCapacity();
            }
        }
        return 0;
    }

    float GetFuelQuantity(int type)
    {
        foreach (ITZ_FS_FuelProfile profile: m_Fuels)
        {
            if (profile && profile.GetLiquidType() == type)
            {
                return profile.GetQuantity();
            }
        }
        return 0;
    }

    void SetFuelQuantity(int type, float quantity)
    {
        foreach (ITZ_FS_FuelProfile profile: m_Fuels)
        {
            if (profile && profile.GetLiquidType() == type)
            {
                profile.AddQuantity(quantity);
                SetSynchDirty();
                return;
            }
        }
    }

    void IncrementFuelQuantities()
    {

        foreach (ITZ_FS_FuelProfile profile: m_Fuels)
        {
            if (profile && !profile.IsFull())
            {
                float currentQuantity = profile.GetQuantity();
                float capacity = profile.GetCapacity();

                float minIncrement = profile.GetMin();
                float maxIncrement = profile.GetMax();
                float increment = Math.RandomIntInclusive(minIncrement, maxIncrement);

                profile.AddQuantity(increment);
                SetSynchDirty();
            }
        }
    }

    bool GetSynchDirty()
    {
        return m_SynchDirty;
    }

    void SetSynchDirty(bool dirty = true)
    {
        m_SynchDirty = dirty;
    }

    void SetVariables(ITZ_FS_FuelStationVariables variables)
    {
        m_Variables = variables;
    }

    ITZ_FS_FuelStationVariables GetVariables()
    {
        return m_Variables;
    }
}

class ITZ_FS_FuelStationVariables
{
    float m_RespawnInterval;
    float m_MaxRange;

    bool m_RequiresEnergy;
    bool m_RespawnFuels;
    bool m_CanUseDestroyed;
    bool m_CanMeasure;
}