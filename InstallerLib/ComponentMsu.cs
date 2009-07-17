using System;
using System.ComponentModel;
using System.IO;
using System.Collections.Generic;

namespace InstallerLib
{
    /// <summary>
    /// A component of type "msu" that executes a Windows update package.
    /// </summary>
    public class ComponentMsu : Component
    {
        public ComponentMsu()
            : base("msu")
        {
            m_package = "#APPPATH\\mysetup.msu";
            os_filter_greater = "99";
        }

        private string m_package;
        [Description("Specifies the path to the msu package to execute when installing this component. Can contain path constants (see Help->Path Constant). (REQUIRED)")]
        public string package
        {
            get { return m_package; }
            set { m_package = value; }
        }

        private string m_cmdparameters = "/norestart";
        [Description("Specifies the command line parameters for the wusa.exe program. See MSDN for the command line you can use with wusa.exe. (OPTIONAL)")]
        public string cmdparameters
        {
            get { return m_cmdparameters; }
            set { m_cmdparameters = value; }
        }

        private string m_cmdparameters_basic = "/quiet /norestart";
        [Description("Specifies the command line parameters for the wusa.exe program when running a basic UI mode. Defaults to cmdparameters if blank. See MSDN for the command line you can use with wusa.exe. (OPTIONAL)")]
        public string cmdparameters_basic
        {
            get { return m_cmdparameters_basic; }
            set { m_cmdparameters_basic = value; }
        }

        private string m_cmdparameters_silent = "/quiet /norestart";
        [Description("Specifies the command line parameters for the wusa.exe program when running a silent install. Defaults to cmdparameters if blank. See MSDN for the command line you can use with wusa.exe. (OPTIONAL)")]
        public string cmdparameters_silent
        {
            get { return m_cmdparameters_silent; }
            set { m_cmdparameters_silent = value; }
        }

        protected override void OnXmlWriteTag(XmlWriterEventArgs e)
        {
            e.XmlWriter.WriteAttributeString("package", m_package);
            e.XmlWriter.WriteAttributeString("cmdparameters", m_cmdparameters);
            e.XmlWriter.WriteAttributeString("cmdparameters_silent", m_cmdparameters_silent);
            e.XmlWriter.WriteAttributeString("cmdparameters_basic", m_cmdparameters_basic);
            base.OnXmlWriteTag(e);
        }

        protected override void OnXmlReadTag(XmlElementEventArgs e)
        {
            ReadAttributeValue(e, "package", ref m_package);
            ReadAttributeValue(e, "cmdparameters", ref m_cmdparameters);
            ReadAttributeValue(e, "cmdparameters_silent", ref m_cmdparameters_silent);
            ReadAttributeValue(e, "cmdparameters_basic", ref m_cmdparameters_basic);
            base.OnXmlReadTag(e);
        }
    }
}
