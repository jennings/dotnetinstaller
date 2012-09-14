#include "StdAfx.h"
#include "SetControlValuesTask.h"

SetControlValuesTask::SetControlValuesTask(const htmlayout::dom::element& root, HANDLE evt, std::wstring * perror)
	: m_root(root)
	, m_event(evt)
	, m_perror(perror)
{

}

void SetControlValuesTask::exec(htmlayout::dom::element elt)
{
	if (elt.is_valid())
	{
		const wchar_t * id = elt.get_attribute("id");
		const wchar_t * type = elt.get_attribute("type");
		const wchar_t * control_ptr = elt.get_attribute("control_ptr");
		const wchar_t * disabled_a = elt.get_attribute("disabled");
		bool disabled = (disabled_a != NULL && wcslen(disabled_a) > 0);
		const wchar_t * has_value_disabled_a = elt.get_attribute("has_value_disabled");
		bool has_value_disabled = (has_value_disabled_a != NULL && wcslen(has_value_disabled_a) > 0);

		if (id != NULL && type != NULL && (! disabled || has_value_disabled))
		{
			if (0 == wcscmp(type, L"checkbox") && elt.get_attribute("license") != NULL)
			{
				ControlLicense * p_license = reinterpret_cast<ControlLicense *>(DVLib::wstring2long(control_ptr, 16));
				if (! InstallUILevelSetting::Instance->IsSilent())
				{
					if (! elt.get_state(STATE_CHECKED))
					{
						THROW_EX(p_license->accept_message);
					}
				}

				std::wstring value = elt.get_state(STATE_CHECKED) ? L"1" : L"0";
				LOG(L"--- Setting user-defined license value '" << id << L"'=" << value);
				InstallerSession::Instance->AdditionalControlArgs[id] = value;
			}
			else if (0 == wcscmp(type, L"checkbox"))
			{
				std::wstring value;
				if (control_ptr != NULL)
				{
					ControlCheckBox * p_checkbox = reinterpret_cast<ControlCheckBox *>(DVLib::wstring2long(control_ptr, 16));
					value = elt.get_state(STATE_CHECKED) ? p_checkbox->checked_value.GetValue() : p_checkbox->unchecked_value.GetValue();
				}
				else
				{
					value = elt.get_state(STATE_CHECKED) ? L"1" : L"0";
				}

				LOG(L"--- Setting user-defined checkbox value '" << id << L"'=" << value);
				InstallerSession::Instance->AdditionalControlArgs[id] = value;
			}
			else if (0 == wcscmp(type, L"radio"))
			{
				std::wstring value;
				// Ignore this radio button if it's not checked.
				// Only one "checked" element for each ID should exist
				if (elt.get_state(STATE_CHECKED))
				{
					const wchar_t * elt_value = elt.get_attribute("value");
					value = elt_value != NULL ? elt_value : L"1";

					LOG(L"--- Setting user-defined radio button value '" << id << L"'=" << value);
					InstallerSession::Instance->AdditionalControlArgs[id] = value;
				}
			}
			else if (0 == wcscmp(type, L"text") || (0 == wcscmp(type, L"password"))
			{
				std::wstring value = elt.get_value().to_string();
				LOG(L"--- Setting user-defined edit value '" << id << L"'=" << value);
				InstallerSession::Instance->AdditionalControlArgs[id] = value;
			}
			else if (0 == wcscmp(type, L"file-path") || 0 == wcscmp(type, L"folder-path"))
			{
				std::wstring value = elt.get_value().to_string();
				if (value.empty()) value = elt.get_attribute("novalue");
				value = DVLib::StripPathTerminator(value);
				LOG(L"--- Setting user-defined browse value '" << id << L"'=" << value);
				InstallerSession::Instance->AdditionalControlArgs[id] = value;
			}

			// TODO: support widget types: select, dropdown-select, textarea, htmlarea
		}

		for(unsigned int child_index = 0; child_index < elt.children_count(); child_index++)
		{
			exec(elt.child(child_index));
		}
	}
}
	
void SetControlValuesTask::exec()
{
	try
	{
		exec(m_root);
	}
	catch(std::exception& ex)
	{
		TRYLOG(L"Error: " << DVLib::string2wstring(ex.what()));
		if (NULL != m_perror)
		{
			* m_perror = DVLib::string2wstring(ex.what());
		}
	}

	CHECK_WIN32_BOOL(SetEvent(m_event),
		L"SetEvent");
}
