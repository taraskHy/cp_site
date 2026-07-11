import requests
import streamlit as st
from parser import Status


def fetch_user(list_of_ids, handle):
    if not handle:
        st.warning("No Codeforces handle was provided.")
        return None

    url = "https://codeforces.com/api/user.status"

    params = {
        "handle": handle,
        "from": 1,
        "count": 1000000000
    }

    try:
        response = requests.get(
            url,
            params=params,
            timeout=10
        )

        if response.status_code >= 500:
            st.error(
                "The Codeforces API appears to be unavailable. "
                f"HTTP status: {response.status_code}"
            )
            return None

        response.raise_for_status()
        response_json = response.json()

    except requests.exceptions.Timeout:
        st.error("The Codeforces API request timed out. Try again later.")
        return None

    except requests.exceptions.ConnectionError:
        st.error("Could not connect to the Codeforces API.")
        return None

    except requests.exceptions.HTTPError as error:
        st.error(f"Codeforces returned an HTTP error: {error}")
        return None

    except requests.exceptions.RequestException as error:
        st.error(f"The Codeforces API request failed: {error}")
        return None

    except ValueError:
        st.error("Codeforces returned an invalid response.")
        return None

    if response_json.get("status") != "OK":
        comment = response_json.get(
            "comment",
            "Unknown Codeforces API error"
        )

        st.error(f"Codeforces API error: {comment}")
        return None

    condensed = {}

    for result in reversed(response_json.get("result", [])):
        problem = result.get("problem", {})

        problem_id = (
            str(result.get("contestId", ""))
            + problem.get("index", "")
        )

        if problem_id not in list_of_ids:
            continue

        verdict = result.get("verdict")

        if verdict == "OK":
            condensed[problem_id] = Status.AC
        elif verdict is None:
            condensed[problem_id] = Status.NAT
        else:
            condensed[problem_id] = Status.AT

    for problem_id in list_of_ids:
        if problem_id not in condensed:
            condensed[problem_id] = Status.NAT

    return condensed


if __name__ == "__main__":
    result = fetch_user(
        ["2042C", "808G"],
        "Tomerh1307"
    )

    if result is not None:
        st.write(result)