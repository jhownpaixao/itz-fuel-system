class ITZ_FS_FuelStation
{
    string m_ID;
    vector m_Position;
    ref ITZ_FS_FuelStationVariables m_Variables;
    
    ref array<ref ITZ_FS_FuelProfile> m_Fuels;

    [NonSerialized()]
    ref Timer m_RespawnTimer;

    [NonSerialized()]
    bool m_SynchDirty;

    void ITZ_FS_FuelStation()
    {
        m_Fuels = new array<ref ITZ_FS_FuelProfile>();
        m_SynchDirty = false;
    }

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
                Print("IncrementFuelQuantities: Incrementando combustível do tipo " + profile.GetFuelType() + " em " + increment + " litros. Quantidade atual: " + currentQuantity + "/" + capacity);
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

    void RespawnFuels()
    {
        if(m_Variables.m_RespawnFuels)
        {
            Print("RespawnFuels: Iniciando respawn de combustível para a estação " + m_ID);
            if (m_RespawnTimer)
                m_RespawnTimer.Stop();

            float interval = m_Variables.m_RespawnInterval;
            if(interval <= 10)
                interval = 10.0;

            m_RespawnTimer = new Timer(CALL_CATEGORY_GAMEPLAY);
            m_RespawnTimer.Run(interval, this, "IncrementFuelQuantities", null, true);
        }
        else
        {
            Print("RespawnFuels: Respawn de combustível desativado para a estação " + m_ID);
            foreach (ITZ_FS_FuelProfile profile: m_Fuels)
            {
                if (profile)
                {
                    float capacity = profile.GetCapacity();

                    float minIncrement = profile.GetMin();
                    float maxIncrement = profile.GetMax();
                    float increment = Math.RandomIntInclusive(minIncrement, maxIncrement);

                    if(increment > capacity)
                        increment = capacity;

                    profile.SetQuantity(increment);
                    Print("RespawnFuels: Definindo quantidade inicial de combustível do tipo " + profile.GetFuelType() + " para " + increment + " litros.");
                    
                    SetSynchDirty();
                }
            }
        }

       
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

    void ITZ_FS_FuelStationVariables()
    {
        m_RespawnInterval = 300.0;
        m_MaxRange = 50.0;
        m_RequiresEnergy = true;
        m_RespawnFuels = true;
        m_CanUseDestroyed = true;
        m_CanMeasure = true;
    }
}