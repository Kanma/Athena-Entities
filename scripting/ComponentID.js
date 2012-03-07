/** @file   ComponentID.js
    @author Philip Abbet

    Declaration of the class Athena.Entities.ComponentID

    @note   Using a pure JavaScript class here instead of a binding over
            the C++ one results in faster code.
*/

Athena.Entities.ComponentID = function()
{
    if (arguments.length == 3)
    {
        this.type      = arguments[0];
        this.entity    = arguments[1];
        this.component = arguments[2];
    }
    else if (arguments.length == 2)
    {
        this.type      = arguments[0];
        this.entity    = '';
        this.component = arguments[1];
    }
    else if ((arguments.length == 1) && (typeof(arguments[0]) === 'number'))
    {
        this.type      = arguments[0];
        this.entity    = '';
        this.component = '';
    }
    else if ((arguments.length == 1) && (typeof(arguments[0]) === 'string'))
    {
        this.type      = Athena.Entities.COMP_NONE;
        this.entity    = '';
        this.component = '';

        var index = arguments[0].indexOf('://');
        if (index >= 0)
        {
            var index2 = arguments[0].indexOf(':', index + 3);
            if (index2 >= 0)
            {
                this.entity    = arguments[0].substring(index + 3, index2);
                this.component = arguments[0].substr(index2 + 1);
            }
            else
            {
                this.component = arguments[0].substr(index + 3);
            }

            var prefix = arguments[0].substr(0, index);
            if (prefix === 'Transforms')
                this.type = Athena.Entities.COMP_TRANSFORMS;
            else if (prefix === 'Visual')
                this.type = Athena.Entities.COMP_VISUAL;
            else if (prefix === 'Audio')
                this.type = Athena.Entities.COMP_AUDIO;
            else if (prefix === 'Physical')
                this.type = Athena.Entities.COMP_PHYSICAL;
            else if (prefix === 'Debug')
                this.type = Athena.Entities.COMP_DEBUG;
            else if (prefix === 'Other')
                this.type = Athena.Entities.COMP_OTHER;
        }
    }
    else
    {
        throw 'Invalid parameters, valid syntaxes:\nComponentID(type, entity, component)\nComponentID(type, component)\nComponentID(type)\nComponentID(id)';
    }
}


/**************************************** METHODS **************************************/

Athena.Entities.ComponentID.prototype.equals = function(id)
{
    return (this.type === id.type) && (this.entity === id.entity) && (this.component === id.component);
}

//-----------------------------------------------------------------------

Athena.Entities.ComponentID.prototype.toString = function()
{
    var suffix = this.component;
    if (this.entity !== '')
        suffix = this.entity + ':' + suffix;

    if (this.type === Athena.Entities.COMP_TRANSFORMS)
        return "Transforms://" + suffix;
    else if (this.type === Athena.Entities.COMP_VISUAL)
        return "Visual://" + suffix;
    else if (this.type === Athena.Entities.COMP_AUDIO)
        return "Audio://" + suffix;
    else if (this.type === Athena.Entities.COMP_PHYSICAL)
        return "Physical://" + suffix;
    else if (this.type === Athena.Entities.COMP_DEBUG)
        return "Debug://" + suffix;
    else if (this.type === Athena.Entities.COMP_OTHER)
        return "Other://" + suffix;

    return "None";
}


/**************************************** OTHERS ***************************************/

Athena.Entities.ComponentID.prototype.__classname__ = 'Athena.Entities.ComponentID';
